// Windows supplies PowerShell and ZipArchive. The worker is embedded in the
// single-file mod, launched without a console, and never executes archive code.
#include "launchpad_backup_script.inc"
constexpr UINT kFullBackupDone=WM_APP+65;
struct FullBackupJob {
    HANDLE process=nullptr,thread=nullptr;
    std::wstring work,mode,archive;
};
[[clang::no_destroy]] std::unique_ptr<FullBackupJob> g_fullBackupJob;

std::wstring BackupJsonString(const std::wstring& value) {
    std::wstring out=L"\"";
    for(WCHAR c:value) {
        if(c==L'"'||c==L'\\'){out+=L'\\';out+=c;}
        else if(c<32){WCHAR escaped[8]{};swprintf_s(escaped,L"\\u%04x",(unsigned)c);out+=escaped;}
        else out+=c;
    }
    return out+L"\"";
}
bool BackupWriteUtf8(const std::wstring& path,const std::wstring& text) {
    int size=WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,text.data(),(int)text.size(),nullptr,0,nullptr,nullptr);
    if(size<=0)return false;
    std::string bytes(size,'\0');
    WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,text.data(),(int)text.size(),bytes.data(),size,nullptr,nullptr);
    HANDLE file=CreateFileW(path.c_str(),GENERIC_WRITE,0,nullptr,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(file==INVALID_HANDLE_VALUE)return false;
    DWORD written=0;
    bool ok=WriteFile(file,bytes.data(),(DWORD)bytes.size(),&written,nullptr)&&written==bytes.size();
    CloseHandle(file);return ok;
}
std::wstring BackupReadResult(const std::wstring& work,PCWSTR key,bool encoded=true) {
    auto value=launchpad_settings_store::Read(work+L"\\result.ini",L"result",key);
    if(!encoded)return value;
    std::wstring decoded;
    return launchpad_settings_store::Decode(value,decoded)?decoded:L"";
}
void BackupDeleteWork(const std::wstring& work) {
    WCHAR temporary[32768]{};GetTempPathW(ARRAYSIZE(temporary),temporary);
    auto parent=std::filesystem::path(work).parent_path().lexically_normal().wstring();
    auto temp=std::filesystem::path(temporary).lexically_normal().wstring();
    while(!temp.empty()&&(temp.back()==L'\\'||temp.back()==L'/'))temp.pop_back();
    if(_wcsicmp(parent.c_str(),temp.c_str())||!std::filesystem::path(work).filename().wstring().starts_with(L"RYJSBackup-"))return;
    std::error_code error;
    std::filesystem::remove_all(work,error);
}
DWORD WINAPI FullBackupWait(void* parameter) {
    auto* job=(FullBackupJob*)parameter;
    WaitForSingleObject(job->process,INFINITE);
    HWND window=g_wnd;
    if(window)PostMessageW(window,kFullBackupDone,0,0);
    return 0;
}
void StopFullBackupJob() {
    if(!g_fullBackupJob)return;
    WaitForSingleObject(g_fullBackupJob->process,INFINITE);
    if(g_fullBackupJob->thread){WaitForSingleObject(g_fullBackupJob->thread,INFINITE);CloseHandle(g_fullBackupJob->thread);}
    CloseHandle(g_fullBackupJob->process);
    BackupDeleteWork(g_fullBackupJob->work);
    g_fullBackupJob.reset();g_fullBackupBusy=false;
}
bool StartFullBackupJob(const std::wstring& mode,const std::wstring& archive,const std::wstring& expected=L"") {
    if(g_fullBackupJob)return false;
    auto job=std::make_unique<FullBackupJob>();job->mode=mode;job->archive=archive;
    WCHAR temporary[32768]{},id[40]{},module[32768]{},storage[32768]{},system[32768]{};
    GUID guid{};if(FAILED(CoCreateGuid(&guid)))return false;
    StringFromGUID2(guid,id,ARRAYSIZE(id));
    if(!GetTempPathW(ARRAYSIZE(temporary),temporary)||!GetModuleFileNameW((HINSTANCE)&__ImageBase,module,ARRAYSIZE(module))||
        !Wh_GetModStoragePath(storage,ARRAYSIZE(storage))||!GetSystemDirectoryW(system,ARRAYSIZE(system)))return false;
    job->work=std::wstring(temporary)+L"RYJSBackup-"+id;
    if(!CreateDirectoryW(job->work.c_str(),nullptr))return false;
    auto modRoot=std::filesystem::path(storage);
    auto modId=modRoot.filename().wstring();
    auto source=modRoot.parent_path().parent_path().parent_path().parent_path()/L"ModsSource"/(modId+L".wh.cpp");
    std::wstring preview=L"false";
#ifdef SEAMLESS_PREVIEW
    preview=L"true";modId=L"ryjs-launchpad";
    source=std::filesystem::path(module).parent_path().parent_path()/L"ryjs-launchpad.wh.cpp";
#endif
    std::wstring config=L"{\"mode\":"+BackupJsonString(mode)+L",\"archive\":"+BackupJsonString(archive)+
        L",\"expected_hash\":"+BackupJsonString(expected)+L",\"storage\":"+BackupJsonString(DataRoot())+
        L",\"mod_id\":"+BackupJsonString(modId)+L",\"version\":"+BackupJsonString(WH_MOD_VERSION)+
        L",\"source\":"+BackupJsonString(source.wstring())+L",\"binary\":"+BackupJsonString(module)+L",\"preview\":"+preview;
    std::wstring host=L"{";
    bool first=true;
    for(const auto& field:Set_MakeFields()) {
        if(!first)host+=L",";
        first=false;
#ifdef SEAMLESS_PREVIEW
        auto baseline=Preview::Setting(field.key);
#else
        std::wstring baseline=WindhawkUtils::StringSetting::make(field.key).get();
#endif
        host+=BackupJsonString(field.key)+L":"+BackupJsonString(baseline);
    }
    config+=L",\"defaults\":"+std::wstring(kFullBackupDefaults)+L",\"host_settings\":"+host+L"}}";
    auto script=job->work+L"\\worker.ps1",jobFile=job->work+L"\\job.json";
    if(!BackupWriteUtf8(script,L"\uFEFF"+std::wstring(kFullBackupScript))||!BackupWriteUtf8(jobFile,config)) {
        BackupDeleteWork(job->work);return false;
    }
    auto executable=std::wstring(system)+L"\\WindowsPowerShell\\v1.0\\powershell.exe";
    auto command=L"\""+executable+L"\" -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass -File \""+script+L"\" -JobPath \""+jobFile+L"\"";
    STARTUPINFOW startup{sizeof(startup)};startup.dwFlags=STARTF_USESHOWWINDOW;startup.wShowWindow=SW_HIDE;
    PROCESS_INFORMATION process{};
    if(!CreateProcessW(executable.c_str(),command.data(),nullptr,nullptr,FALSE,CREATE_NO_WINDOW,nullptr,job->work.c_str(),&startup,&process)) {
        BackupDeleteWork(job->work);return false;
    }
    CloseHandle(process.hThread);job->process=process.hProcess;
    job->thread=CreateThread(nullptr,0,FullBackupWait,job.get(),0,nullptr);
    if(!job->thread) {
        WaitForSingleObject(job->process,INFINITE);CloseHandle(job->process);BackupDeleteWork(job->work);return false;
    }
    g_fullBackupJob=std::move(job);g_fullBackupBusy=true;
    if(g_shown)Render();return true;
}
void BackupResumeAfterImport(bool succeeded) {
    g_fullImportBusy=false;g_workerStop=false;g_webStop=false;
    if(succeeded) {
        WritePrivateProfileStringW(nullptr,nullptr,nullptr,(DataRoot()+L"\\ui-settings.ini").c_str());
#ifdef SEAMLESS_PREVIEW
        for(const auto& field:Set_MakeFields()) {
            auto stored=launchpad_settings_store::Read(DataRoot()+L"\\ui-settings.ini",L"values",field.key);
            std::wstring value;if(!stored.empty()&&launchpad_settings_store::Decode(stored,value))Preview::PutSetting(field.key,value);
        }
#endif
        g_dataReadOnly=g_layoutReadOnly=g_entriesReadOnly=false;
        g_appsKey=-1;g_appsLoaded=false;g_appsTime=0;
        LoadSettings();g_labelFormat.Reset();StartLoadingApps();Open();
    }
}
bool BackupCloseSettings() {
    g_settingsStop=true;
    if(HWND window=g_settingsWnd)PostMessageW(window,WM_CLOSE,0,0);
    if(g_settingsThread) {
        if(WaitForSingleObject(g_settingsThread,15000)!=WAIT_OBJECT_0)return false;
        CloseHandle(g_settingsThread);g_settingsThread=nullptr;
    }
    return true;
}
void OnFullBackupDone(HWND owner) {
    if(!g_fullBackupJob)return;
    auto mode=g_fullBackupJob->mode,archive=g_fullBackupJob->archive;
    bool ok=BackupReadResult(g_fullBackupJob->work,L"status",false)==L"ok";
    auto summary=BackupReadResult(g_fullBackupJob->work,L"summary");
    auto hash=BackupReadResult(g_fullBackupJob->work,L"hash",false);
    auto previous=BackupReadResult(g_fullBackupJob->work,L"previous");
    StopFullBackupJob();
    if(summary.empty())summary=Tr(L"The backup helper did not return a result.");
    bool oldModal=g_modal;g_modal=true;
    if(!ok) {
        MessageBoxW(owner,(std::wstring(Tr(L"Complete backup/import failed:"))+L"\n\n"+summary).c_str(),Tr(L"Backup and restore"),MB_OK|MB_ICONERROR);
        if(mode==L"import")BackupResumeAfterImport(false);
    } else if(mode==L"inspect") {
        auto message=std::wstring(Tr(L"Backup verified:"))+L"\n"+summary+L"\n\n"+
            Tr(L"Replace the current layout, icons and settings? The previous data will be kept for rollback.");
        if(MessageBoxW(owner,message.c_str(),Tr(L"Import complete backup"),MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDYES) {
            if(!BackupCloseSettings()) {
                MessageBoxW(owner,Tr(L"Close the settings window and try importing again."),Tr(L"Backup and restore"),MB_OK|MB_ICONERROR);
                g_modal=oldModal;return;
            }
            HideNow();g_fullImportBusy=true;
            WritePrivateProfileStringW(nullptr,nullptr,nullptr,(DataRoot()+L"\\ui-settings.ini").c_str());
            g_workerStop=true;
            if(g_worker){WaitForSingleObject(g_worker,INFINITE);CloseHandle(g_worker);g_worker=nullptr;}
            MSG queued{};
            while(PeekMessageW(&queued,nullptr,WM_APP_APPS,WM_APP_APPS,PM_REMOVE))delete (AppList*)queued.lParam;
            StopWebIcons();
            if(!StartFullBackupJob(L"import",archive,hash)) {
                BackupResumeAfterImport(false);
                MessageBoxW(owner,Tr(L"Could not start the backup helper."),Tr(L"Backup and restore"),MB_OK|MB_ICONERROR);
            }
        }
    } else {
        auto message=std::wstring(Tr(mode==L"export"?L"Complete backup saved and verified:":L"Complete backup imported:"))+L"\n\n"+summary;
        if(!previous.empty())message+=L"\n\n"+std::wstring(Tr(L"Previous data:"))+L"\n"+previous;
        MessageBoxW(owner,message.c_str(),Tr(L"Backup and restore"),MB_OK|MB_ICONINFORMATION);
        if(mode==L"import")BackupResumeAfterImport(true);
    }
    g_modal=oldModal;if(g_shown)Render();
}
void ShowFullBackupDialog(HWND owner,bool importing) {
    if(g_fullBackupJob) {
        MessageBoxW(owner,Tr(L"A backup or import is already running."),Tr(L"Backup and restore"),MB_OK|MB_ICONINFORMATION);return;
    }
    WCHAR file[32768]{};
    if(!importing) {
        SYSTEMTIME now{};GetLocalTime(&now);
        swprintf_s(file,L"RYJS-Launchpad-%04u%02u%02u-%02u%02u%02u.ryjs.zip",now.wYear,now.wMonth,now.wDay,now.wHour,now.wMinute,now.wSecond);
    }
    PWSTR desktop=nullptr;SHGetKnownFolderPath(FOLDERID_Desktop,0,nullptr,&desktop);
    auto filter=std::wstring(Tr(L"Complete Launchpad backups"))+L'\0'+L"*.zip"+L'\0'+L'\0';
    OPENFILENAMEW dialog{sizeof(dialog)};dialog.hwndOwner=owner;dialog.lpstrFile=file;dialog.nMaxFile=ARRAYSIZE(file);
    dialog.lpstrFilter=filter.c_str();dialog.lpstrDefExt=L"zip";dialog.lpstrInitialDir=desktop;
    dialog.lpstrTitle=Tr(importing?L"Import complete backup":L"Create complete backup");
    dialog.Flags=OFN_NOCHANGEDIR|OFN_EXPLORER|(importing?OFN_FILEMUSTEXIST:OFN_OVERWRITEPROMPT);
    bool oldModal=g_modal;g_modal=true;
    BOOL selected=importing?GetOpenFileNameW(&dialog):GetSaveFileNameW(&dialog);
    CoTaskMemFree(desktop);g_modal=oldModal;
    if(!selected)return;
    if(!importing && !g_layoutReadOnly && !g_dataReadOnly && !SaveTiles()) {
        MessageBoxW(owner,Tr(L"Could not save the current layout before backup."),Tr(L"Backup and restore"),MB_OK|MB_ICONERROR);return;
    }
    if(!StartFullBackupJob(importing?L"inspect":L"export",file))MessageBoxW(owner,Tr(L"Could not start the backup helper."),Tr(L"Backup and restore"),MB_OK|MB_ICONERROR);
}

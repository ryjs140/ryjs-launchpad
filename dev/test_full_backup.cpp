#define SEAMLESS_PREVIEW 1
#include "../mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp"
#include <cstdio>
void Preview_RunSmoke(HWND) {}
int checks=0,failures=0;
void Check(bool value,const char* label) {
    ++checks;
    if(!value){++failures;printf("FAIL: %s\n",label);}
}
bool Finish(const char* label) {
    if(!g_fullBackupJob)return false;
    bool done=WaitForSingleObject(g_fullBackupJob->process,45000)==WAIT_OBJECT_0;
    Check(done,"helper completes within timeout");
    auto status=BackupReadResult(g_fullBackupJob->work,L"status",false);
    if(status!=L"ok")fwprintf(stderr,L"Worker: %ls\n",BackupReadResult(g_fullBackupJob->work,L"summary").c_str());
    Check(status==L"ok",label);
    return done&&status==L"ok";
}
int wmain() {
    CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    WCHAR executable[32768]{};GetModuleFileNameW(nullptr,executable,ARRAYSIZE(executable));
    auto root=std::filesystem::path(executable).parent_path()/(L"native-backup-"+std::to_wstring(GetTickCount64()));
    std::filesystem::create_directories(root/L"data-v1");
    Preview::directory=(root/L"data-v1").wstring();
    for(const auto& field:Set_MakeFields())Preview::PutSetting(field.key,field.def);
    Preview::PutSetting(L"appSource",L"pinned");Preview::PutSetting(L"gridLayout",L"groups");
    Preview::PutSetting(L"gridIconSize",L"96");
    LoadSettings();g_appsKey=AppsKey();
    auto layout=std::vector<std::wstring>{L"schema\t1\tgroups",L"folder\t备份测试",L"in\tryjs-entry:fixture"};
    Check(WriteLines(StoragePath(L"groups-v1-pinned.txt"),layout),"native Unicode layout fixture written");
    Check(WriteLines(StoragePath(L"pins.txt"),{L"pin\tryjs-entry:fixture"}),"native pins fixture written");
    Check(WriteLines(StoragePath(L"entries-v1.txt"),{L"schema\t1\tentries",L"entry\tryjs-entry:fixture\t1\thttps://example.org/\t自定义名称\t\t"}),"native alias fixture written");
    Check(WriteLines(StoragePath(L"display-names-v1.txt"),{L"schema\t1\tdisplay-names",L"name\tryjs-entry:fixture\tSource name\t备份别名"}),"display name fixture written");
    Check(WriteLines(StoragePath(L"group-geometry-layout-pinned.txt"),{L"schema\t2\tgroup-geometry",L"group\t0\t备份测试\t24\t0\t300\t280\tgroup\t3\t2"}),"integer geometry v2 fixture written");
    auto archive=(root/L"完整备份.ryjs.zip").wstring();
    Check(StartFullBackupJob(L"export",archive),"GUI bridge starts embedded export worker");
    Finish("GUI bridge exports a verified complete archive");
    auto hash=BackupReadResult(g_fullBackupJob->work,L"hash",false);
    auto temporary=g_fullBackupJob->work;
    StopFullBackupJob();
    Check(!std::filesystem::exists(temporary)&&!g_fullBackupBusy,"native worker handles and temporary data cleaned");
    Check(StartFullBackupJob(L"inspect",archive),"GUI bridge starts validation worker");
    Finish("GUI bridge validates its own archive");
    Check(BackupReadResult(g_fullBackupJob->work,L"hash",false)==hash,"confirmation binds to exact archive hash");
    StopFullBackupJob();
    Check(WriteLines(StoragePath(L"groups-v1-pinned.txt"),{L"schema\t1\tgroups",L"folder\tReplaced"}),"current layout changed before import");
    Preview::PutSetting(L"gridIconSize",L"56");
    g_fullImportBusy=true;
    Check(!SaveTiles(),"layout writes blocked during import");
    StartLoadingApps();Check(!g_worker,"app enumeration blocked during import");
    Open();Check(!g_shown,"launcher cannot reopen during import");
    Check(StartFullBackupJob(L"import",archive,hash),"GUI bridge starts confirmed import");
    Finish("GUI bridge imports verified archive");
    auto previous=BackupReadResult(g_fullBackupJob->work,L"previous");
    StopFullBackupJob();g_fullImportBusy=false;
    Check(!previous.empty()&&std::filesystem::exists(previous),"previous data retained for rollback");
    Check(ReadLines(StoragePath(L"groups-v1-pinned.txt"))==std::vector<std::vector<std::wstring>>{{L"schema",L"1",L"groups"},{L"folder",L"备份测试"},{L"in",L"ryjs-entry:fixture"}},"Unicode layout survives native export and import");
    Check(!std::filesystem::exists(root/L"data-v1"/L"binary"),"archived executable is never installed");
    Check(ReadLines(StoragePath(L"display-names-v1.txt"))[1][3]==L"备份别名","local display names survive native full backup restore");
    auto restoredGeometry=ReadLines(StoragePath(L"group-geometry-layout-pinned.txt"));
    Check(restoredGeometry[0][1]==L"2"&&restoredGeometry[1][8]==L"3"&&restoredGeometry[1][9]==L"2","integer cell counts survive native full backup restore");
    const auto settingsFile=StoragePath(L"ui-settings.ini");
    using launchpad_settings_store::Resolve;
    Check(Resolve(settingsFile,L"gridIconSize",L"56")==L"96","backup restores the icon size over the destination setting");
    Check(launchpad_settings_store::Save(settingsFile,{{L"gridIconSize",L"128",L"56"}})&&
          Resolve(settingsFile,L"gridIconSize",L"56")==L"128","built-in settings can change the restored icon size");
    Check(Resolve(settingsFile,L"gridIconSize",L"72")==L"72","a Windhawk size change replaces the restored override");
    printf("Native full-backup checks: %d, failures: %d\n",checks,failures);
    return failures?1:0;
}

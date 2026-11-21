// One native dialog for files, shortcuts, URLs, Store apps and existing aliases.
struct EntryEditor {
    HWND fields[5]{},groups{};
    std::vector<std::shared_ptr<Folder>> destinations;
    std::shared_ptr<App> app;
    bool done=false,accepted=false;
};
constexpr WCHAR kEntryEditorClass[]=L"RYJSLaunchpadEntryEditor";
std::wstring EntryText(HWND field) {
    std::wstring value(GetWindowTextLengthW(field)+1,L'\0');
    value.resize(GetWindowTextW(field,value.data(),(int)value.size())); return value;
}
bool PickEntryFile(HWND owner,std::wstring& path,bool icon=false) {
    if (icon) return PickIconFile(owner,path);
    WCHAR file[32768]{};
    OPENFILENAMEW dialog{sizeof(dialog)}; dialog.hwndOwner=owner;
    dialog.lpstrFilter=L"Programs, shortcuts and Remote Desktop\0*.exe;*.lnk;*.url;*.rdp\0All files\0*.*\0\0";
    dialog.lpstrFile=file; dialog.nMaxFile=ARRAYSIZE(file);
    dialog.Flags=OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
    if (!GetOpenFileNameW(&dialog)) return false;
    path=file; return true;
}
LRESULT CALLBACK EntryEditorProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) {
    auto* state=(EntryEditor*)GetWindowLongPtrW(hwnd,GWLP_USERDATA);
    if (msg==WM_NCCREATE) {state=(EntryEditor*)((CREATESTRUCTW*)lp)->lpCreateParams; SetWindowLongPtrW(hwnd,GWLP_USERDATA,(LONG_PTR)state);}
    if (state && msg==WM_COMMAND) {
        auto id=LOWORD(wp);
        if (id==IDCANCEL) {state->done=true;return 0;}
        if (id==IDOK) {
            auto target=EntryText(state->fields[1]),name=EntryText(state->fields[0]);
            if (target.empty() || name.empty() || name.find_first_of(L"\t\r\n")!=std::wstring::npos) {
                MessageBoxW(hwnd,Tr(L"Enter a name and a valid target."),Tr(L"Add or edit entry"),MB_OK|MB_ICONINFORMATION);return 0;
            }
            if (!IsWebEntry(target) && !ToLower(target).starts_with(L"shell:AppsFolder\\") && GetFileAttributesW(target.c_str())==INVALID_FILE_ATTRIBUTES) {
                if (MessageBoxW(hwnd,Tr(L"The target is missing. Keep it so you can repair it later?"),Tr(L"Add or edit entry"),MB_YESNO|MB_ICONQUESTION)!=IDYES) return 0;
            }
            state->accepted=state->done=true;return 0;
        }
        if (id==30 || id==31) {
            std::wstring path;
            if (PickEntryFile(hwnd,path,id==31)) {
                SetWindowTextW(state->fields[id==30?1:4],path.c_str());
                if (id==30 && EntryText(state->fields[0]).empty()) SetWindowTextW(state->fields[0],std::filesystem::path(path).stem().c_str());
            }
            return 0;
        }
        if (id==32) {
            AppList installed; AddStartApps(IconPixels(),installed);
            HMENU menu=CreatePopupMenu(); AppList packaged;
            for (const auto& app:installed) if (IsPackagedTarget(app->target)) {
                packaged.push_back(app); AppendMenuW(menu,MF_STRING,(UINT_PTR)packaged.size(),MenuText(app->name).c_str());
            }
            RECT r{};GetWindowRect(GetDlgItem(hwnd,32),&r);
            int choice=TrackLaunchpadPopupMenu(menu,TPM_RETURNCMD|TPM_NONOTIFY,r.left,r.bottom,hwnd,nullptr);
            DestroyMenu(menu);
            if (choice>0 && choice<=(int)packaged.size()) {
                auto app=packaged[choice-1];SetWindowTextW(state->fields[0],app->name.c_str());SetWindowTextW(state->fields[1],app->target.c_str());
            }
            return 0;
        }
    }
    if (state && msg==WM_CLOSE) {state->done=true;return 0;}
    return DefWindowProcW(hwnd,msg,wp,lp);
}
void EditEntry(HWND owner,const std::shared_ptr<App>& existing=nullptr,const std::shared_ptr<Folder>& destination=nullptr) {
    if (g_entriesReadOnly || g_layoutReadOnly || g_dataReadOnly) {
        MessageBoxW(owner,Tr(L"Data is read-only. Resolve the load error before editing."),Tr(L"Add or edit entry"),MB_OK|MB_ICONWARNING);return;
    }
    EntryEditor state;state.app=existing;
    WNDCLASSEXW wc{sizeof(wc)};wc.lpfnWndProc=EntryEditorProc;wc.hInstance=(HINSTANCE)&__ImageBase;
    wc.hCursor=LoadCursorW(nullptr,IDC_ARROW);wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);wc.lpszClassName=kEntryEditorClass;
    RegisterClassExW(&wc);
    UINT dpi=GetDpiForWindow(owner);auto px=[&](int dip){return MulDiv(dip,dpi,96);};
    RECT bounds{};GetWindowRect(owner,&bounds);
    HWND window=CreateWindowExW(WS_EX_DLGMODALFRAME|WS_EX_CONTROLPARENT,kEntryEditorClass,Tr(L"Add or edit entry"),WS_CAPTION|WS_SYSMENU,
        (bounds.left+bounds.right-px(650))/2,(bounds.top+bounds.bottom-px(420))/2,px(650),px(420),owner,nullptr,wc.hInstance,&state);
    if (!window) return;
    HFONT font=CreateFontW(-px(14),0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH,g_settings.font.c_str());
    auto control=[&](PCWSTR cls,PCWSTR text,DWORD style,int x,int y,int w,int h,int id) {
        HWND c=CreateWindowExW(wcscmp(cls,L"EDIT")==0?WS_EX_CLIENTEDGE:0,cls,text,WS_CHILD|WS_VISIBLE|style,px(x),px(y),px(w),px(h),window,(HMENU)(INT_PTR)id,wc.hInstance,nullptr);
        SendMessageW(c,WM_SETFONT,(WPARAM)font,TRUE);return c;
    };
    PCWSTR labels[]={L"Display name",L"File or web address",L"Launch arguments",L"Working directory",L"Custom icon (optional)"};
    std::wstring values[5];
    if (existing) {
        values[0]=existing->name;
        values[1]=existing->target;
        values[2]=existing->arguments;
        values[3]=existing->workingDirectory;
        auto it=g_customIcons.find(AppKey(*existing));
        if(it!=g_customIcons.end())values[4]=it->second;
    }
    for (int i=0;i<5;i++) {
        control(L"STATIC",Tr(labels[i]),0,18,18+i*46,160,24,0);
        state.fields[i]=control(L"EDIT",values[i].c_str(),WS_TABSTOP|ES_AUTOHSCROLL,182,14+i*46,(i==1||i==4)?346:432,30,10+i);
        SendMessageW(state.fields[i],EM_SETLIMITTEXT,i==0?160:32760,0);
    }
    control(L"BUTTON",Tr(L"Browse…"),WS_TABSTOP,536,60,78,30,30);
    control(L"BUTTON",Tr(L"Browse…"),WS_TABSTOP,536,198,78,30,31);
    control(L"STATIC",Tr(L"Destination group"),0,18,250,160,24,0);
    state.groups=control(L"COMBOBOX",L"",WS_TABSTOP|CBS_DROPDOWNLIST|WS_VSCROLL,182,246,432,180,20);
    int selected=0;
    if (g_settings.groups) for (const auto& tile:g_tiles) if (tile.folder) {
        if (tile.folder==destination) selected=(int)state.destinations.size();
        state.destinations.push_back(tile.folder);SendMessageW(state.groups,CB_ADDSTRING,0,(LPARAM)tile.folder->name.c_str());
    }
    if (state.destinations.empty()) SendMessageW(state.groups,CB_ADDSTRING,0,(LPARAM)Tr(L"End of current grid"));
    SendMessageW(state.groups,CB_SETCURSEL,selected,0); EnableWindow(state.groups,!existing);
    control(L"BUTTON",Tr(L"Choose Store app…"),WS_TABSTOP,18,302,190,32,32);
    control(L"BUTTON",Tr(L"Save"),WS_TABSTOP|BS_DEFPUSHBUTTON,422,302,90,32,IDOK);
    control(L"BUTTON",Tr(L"Cancel"),WS_TABSTOP,524,302,90,32,IDCANCEL);
    bool previousModal=g_modal;g_modal=true;
    EnableWindow(owner,FALSE);ShowWindow(window,SW_SHOW);SetForegroundWindow(window);SetFocus(state.fields[existing?0:1]);
    MSG message{};
    while (!state.done) {
        int result=GetMessageW(&message,nullptr,0,0);
        if(result<=0){if(!result)PostQuitMessage((int)message.wParam);break;}
        if(!IsDialogMessageW(window,&message)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    std::wstring name,target,args,dir,icon;int group=0;
    if (state.accepted) {
        name=EntryText(state.fields[0]);
        target=EntryText(state.fields[1]);
        args=EntryText(state.fields[2]);
        dir=EntryText(state.fields[3]);
        icon=EntryText(state.fields[4]);
        group=(int)SendMessageW(state.groups,CB_GETCURSEL,0,0);
    }
    EnableWindow(owner,TRUE);DestroyWindow(window);DeleteObject(font);g_modal=previousModal;
    if (g_shown) ForceForeground(owner);
    if (!state.accepted) return;
    std::shared_ptr<App> app=existing;
    if (!app) {
        // Same launch target and options reuse the existing identity.
        auto found=std::find_if(g_apps.begin(),g_apps.end(),[&](const auto& a){return (IsWebEntry(target)?a->target==target:ToLower(a->target)==ToLower(target))&&a->arguments==args&&ToLower(a->workingDirectory)==ToLower(dir);});
        if (found!=g_apps.end()) app=*found;
    }
    std::wstring key;
    bool created=!app;
    if (app) key=AppKey(*app);
    else {
        GUID guid{};
        CoCreateGuid(&guid);
        WCHAR id[40]{};
        StringFromGUID2(guid,id,ARRAYSIZE(id));
        key=ToLower(L"ryjs-entry:"+std::wstring(id));
        app=std::make_shared<App>();
    }
    EntryRecord record{created||app->manual,target,name,args,dir};
    bool alreadyPlaced=false;
    for(const auto& tile:g_tiles)if(tile.app==app || (tile.folder&&IndexOf(tile.folder->apps,{app,nullptr})>=0))alreadyPlaced=true;
    auto old=g_entries;g_entries[key]=record;
    if (!SaveEntries()) {g_entries=std::move(old);MessageBoxW(owner,Tr(L"Could not save entry."),Tr(L"Add or edit entry"),MB_OK|MB_ICONERROR);return;}
    ApplyEntryRecord(*app,key,record,IconPixels());
    ApplyDisplayName(*app);
    if (created) g_apps.push_back(app);
    auto previousIcon=g_customIcons.find(key);
    if ((previousIcon==g_customIcons.end() ? !icon.empty() : previousIcon->second!=icon) &&
        !SetCustomIcon(key,icon)) {
        MessageBoxW(owner,Tr(L"Could not save the selected icon."),Tr(L"Add or edit entry"),MB_OK|MB_ICONWARNING);
    }
    auto custom=g_customIcons.find(key);
    if(custom!=g_customIcons.end())
        LoadIconFile(CustomIconPath(custom->second),IconPixels(),*app,!IsManagedIcon(custom->second));
    if (!existing && !alreadyPlaced) {
        if (g_settings.groups && group>=0 && group<(int)state.destinations.size()) MoveAppToGroup({app,nullptr},state.destinations[group],(int)state.destinations[group]->apps.size());
        else {g_hidden.erase(key);if(PinnedOnly()){g_pins.insert(key);SavePins();}if(IndexOf(g_tiles,{app,nullptr})<0)g_tiles.push_back({app,nullptr});}
        SaveTiles();
    }
    g_labelCache.clear();Refresh();
    if (g_customIcons.count(key)==0 && app->pixels.empty()) StartWebIconFetch(*app,true);
}
void DeleteManualEntry(const std::shared_ptr<App>& app) {
    if (!app || !app->manual || g_entriesReadOnly || g_dataReadOnly || g_layoutReadOnly) return;
    auto key=AppKey(*app);auto previous=g_entries;g_entries.erase(key);
    if (!SaveEntries()) {g_entries=std::move(previous);return;}
    for(auto& tile:g_tiles)if(tile.folder)std::erase_if(tile.folder->apps,[&](const Tile& t){return t.app==app;});
    std::erase_if(g_tiles,[&](const Tile& t){return t.app==app;});
    std::erase(g_apps,app);g_pins.erase(key);g_hidden.erase(key);g_customIcons.erase(key);
    SavePins();SaveCustomIcons();SaveTiles();Refresh();
}

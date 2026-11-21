#include "launchpad_group_edit.inc"
// Shared operations for pointer, context-menu and keyboard group editing.
void MoveAppToGroup(const Tile& tile, const std::shared_ptr<Folder>& destination, int position) {
    if (!tile.app || !destination) return;
    std::wstring key = AppKey(*tile.app);
    for (auto& group : g_tiles) if (group.folder) {
        auto& apps = group.folder->apps;
        for (int i = (int)apps.size() - 1; i >= 0; --i) if (apps[i].app && AppKey(*apps[i].app) == key) {
            if (group.folder == destination && i < position) --position;
            apps.erase(apps.begin() + i);
        }
    }
    position = std::clamp(position, 0, (int)destination->apps.size());
    destination->apps.insert(destination->apps.begin() + position, tile);
    g_hidden.erase(key);
    if (PinnedOnly()) { g_pins.insert(key); SavePins(); }
}

void MoveGroup(const std::shared_ptr<Folder>& group, int destination) {
    int from = IndexOf(g_tiles, {nullptr, group});
    if (from < 0) return;
    Tile tile = g_tiles[from]; g_tiles.erase(g_tiles.begin() + from);
    destination = std::clamp(destination, 0, (int)g_tiles.size());
    g_tiles.insert(g_tiles.begin() + destination, tile);
}

struct GroupNameDialog {
    HWND edit = nullptr;
    bool done = false, accepted = false;
    std::wstring initial;
};
constexpr WCHAR kGroupNameClass[] = L"RYJSLaunchpadGroupName";
LRESULT CALLBACK GroupNameProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto* dialog = (GroupNameDialog*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    if (msg == WM_NCCREATE) {
        dialog = (GroupNameDialog*)((CREATESTRUCTW*)lParam)->lpCreateParams;
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)dialog);
    }
    if (dialog && msg == WM_COMMAND && (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)) {
        dialog->accepted = LOWORD(wParam) == IDOK; dialog->done = true; return 0;
    }
    if (dialog && msg == WM_CLOSE) { dialog->done = true; return 0; }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

std::wstring PromptGroupName(HWND owner, const std::wstring& initial,PCWSTR caption=L"Group name",int limit=80) {
    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = GroupNameProc; wc.hInstance = (HINSTANCE)&__ImageBase;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); wc.lpszClassName = kGroupNameClass;
    RegisterClassExW(&wc);
    GroupNameDialog dialog; dialog.initial = initial;
    UINT dpi = GetDpiForWindow(owner);
    auto px = [&](int dip) { return MulDiv(dip, dpi, 96); };
    RECT area{}; GetWindowRect(owner, &area);
    HWND window = CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_CONTROLPARENT, kGroupNameClass, Tr(caption),
        WS_CAPTION | WS_SYSMENU, (area.left + area.right - px(440))/2, (area.top + area.bottom - px(170))/2,
        px(440), px(170), owner, nullptr, wc.hInstance, &dialog);
    if (!window) return {};
#ifdef SEAMLESS_PREVIEW
    if(Preview::uiCheck)SetWindowLongPtrW(window,GWL_EXSTYLE,GetWindowLongPtrW(window,GWL_EXSTYLE)|WS_EX_APPWINDOW);
#endif
    HFONT font = CreateFontW(-px(14), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, g_settings.font.c_str());
    dialog.edit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", initial.c_str(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        px(18), px(20), px(392), px(32), window, (HMENU)10, wc.hInstance, nullptr);
    HWND save = CreateWindowExW(0, L"BUTTON", Tr(L"Save"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
        px(222), px(74), px(90), px(30), window, (HMENU)IDOK, wc.hInstance, nullptr);
    HWND cancel = CreateWindowExW(0, L"BUTTON", Tr(L"Cancel"), WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        px(320), px(74), px(90), px(30), window, (HMENU)IDCANCEL, wc.hInstance, nullptr);
    for (HWND control : {dialog.edit, save, cancel}) SendMessageW(control, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessageW(dialog.edit, EM_SETLIMITTEXT, limit, 0);
    bool previousModal = g_modal; g_modal = true;
    EnableWindow(owner,FALSE);ShowWindow(window, SW_SHOW); SetForegroundWindow(window); SetFocus(dialog.edit);
    SendMessageW(dialog.edit, EM_SETSEL, 0, -1);
    MSG message;
    while (!dialog.done) {
        int result = GetMessageW(&message, nullptr, 0, 0);
        if (result <= 0) { if (!result) PostQuitMessage((int)message.wParam); break; }
        if(message.message==WM_KEYDOWN&&(message.wParam==VK_RETURN||message.wParam==VK_ESCAPE)) {
            dialog.accepted=message.wParam==VK_RETURN;dialog.done=true;continue;
        }
        if (!IsDialogMessageW(window, &message)) { TranslateMessage(&message); DispatchMessageW(&message); }
    }
    std::wstring name;
    if (dialog.accepted) {
        name.resize(GetWindowTextLengthW(dialog.edit) + 1);
        name.resize(GetWindowTextW(dialog.edit, name.data(), (int)name.size()));
        std::erase_if(name, [](WCHAR c) { return c < 32; });
        size_t begin = name.find_first_not_of(L" ");
        name = begin == std::wstring::npos ? L"" : name.substr(begin, name.find_last_not_of(L" ") - begin + 1);
    }
    EnableWindow(owner,TRUE);DestroyWindow(window); DeleteObject(font);
    g_modal = previousModal;
    if (g_shown) SetForegroundWindow(owner);
    return name;
}

void CreateGroup(HWND hwnd) {
    auto name = PromptGroupName(hwnd, Tr(L"New group"));
    if (name.empty()) return;
    auto group = std::make_shared<Folder>(); group->name = std::move(name);
    g_tiles.push_back({nullptr, group}); SaveTiles(); Refresh();
}

// Removes a group. A real group hands its apps to the ungrouped block, which
// keeps them in the launcher. Deleting the ungrouped block itself means "these
// apps are not wanted here": they are removed like "Remove from this grid",
// because moving them into a new inbox would only bring the same block back.
void DeleteGroup(const std::shared_ptr<Folder>& group) {
    if (g_groupPositionsReadOnly || g_layoutReadOnly || g_dataReadOnly) return;
    int index = IndexOf(g_tiles, {nullptr, group});
    if (index >= 0) g_tiles.erase(g_tiles.begin() + index);
    if (group->inbox) {
        for (const auto& tile : group->apps) {
            if (!tile.app) continue;
            if (PinnedOnly()) g_pins.erase(AppKey(*tile.app));
            else g_hidden.insert(AppKey(*tile.app));
        }
        if (PinnedOnly()) SavePins();
    } else if (!group->apps.empty()) {
        auto inbox = InboxGroup();
        size_t offset = inbox->apps.size() + inbox->unavailable.size();
        for (const auto& missing : group->unavailable)
            inbox->unavailable.emplace_back(offset + missing.first, missing.second);
        inbox->apps.insert(inbox->apps.end(), group->apps.begin(), group->apps.end());
    }
    SaveTiles();
    Refresh();
}

void ShowGroupMenu(HWND hwnd, const std::shared_ptr<Folder>& group, POINT point) {
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_STRING, 1, Tr(L"Rename group"));
    AppendMenuW(menu, MF_STRING, 2, Tr(L"Move group earlier"));
    AppendMenuW(menu, MF_STRING, 3, Tr(L"Move group later"));
    AppendMenuW(menu, MF_STRING, 4, Tr(group->inbox?L"Delete these apps":L"Delete group"));
    AppendMenuW(menu, MF_STRING, 5, Tr(L"New group"));
    AppendMenuW(menu, MF_STRING, 6, Tr(g_groupEditMode?L"Finish group editing":L"Edit group layout"));
    AppendMenuW(menu, MF_STRING, 7, Tr(group->collapsed?L"Show as expanded group":L"Show as traditional folder"));
    if(g_groupEditMode)AppendMenuW(menu,MF_STRING,8,Tr(L"Cancel group editing"));
    g_modal = true;
    int command = TrackLaunchpadPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTBUTTON, point.x, point.y, hwnd, nullptr);
    g_modal = false; DestroyMenu(menu);
    if(command==6){ToggleGroupEdit();return;}
    if(command==8){CancelGroupEdit();return;}
    if(g_groupEditMode)return;
    if(command==7){
        if(g_groupPositionsReadOnly||g_layoutReadOnly||g_dataReadOnly)return;
        bool wasFree=g_freeGroupLayout,added=false;
        if(!g_groupPositions.count(group)) {
            auto geometry=std::find_if(g_groupGeometry.begin(),g_groupGeometry.end(),[&](const auto& item){return item.group==group;});
            if(geometry==g_groupGeometry.end())return;
            auto position=PositionFromArea(geometry->area);
            if(geometry->squareCells)position=EditorGroupPosition(*geometry);
            else if(geometry->wholeCells) {
                position.style=4;position.snapped=true;
                position.gridX=GroupGridIndex(position.x,GroupPitchX()/g_layout.scale);
                position.gridY=GroupGridIndex(position.y,GroupPitchY()/g_layout.scale);
                position.gridW=geometry->columns;position.gridH=geometry->visibleRows+1;
            }
            g_groupPositions[group]=position;
            added=true;
        }
        g_freeGroupLayout=true;
        group->collapsed=!group->collapsed;
        if(!SaveGroupPositions()) {
            group->collapsed=!group->collapsed;g_freeGroupLayout=wasFree;
            if(added)g_groupPositions.erase(group);
        }
        Refresh();return;
    }
    if (command == 1) {
        auto name = PromptGroupName(hwnd, group->name);
        if (!name.empty()) group->name = std::move(name);
    } else if (command == 2 || command == 3) {
        MoveGroup(group, IndexOf(g_tiles, {nullptr,group}) + (command == 2 ? -1 : 1));
    } else if (command == 4) {
        DeleteGroup(group);
        return;
    } else if (command == 5) { CreateGroup(hwnd); return; }
    if (command) { SaveTiles(); Refresh(); }
}

void GroupDragMove(int x, int y) {
    g_groupDropFolder.reset(); g_groupDropSlot = -1;
    int index = GroupAtPoint(x, y);
    if (index < 0) return;
    const auto& geometry = g_groupGeometry[index];
    g_groupDropFolder = geometry.group;
    auto area = ProjectGroupRect(geometry.area);
    auto origin=GroupCellsOrigin(geometry);
    float relativeX=x-area.left-(origin.x-geometry.area.left);
    float relativeY=y-area.top-(origin.y-geometry.area.top);
    int col=std::clamp((int)std::floor(relativeX/geometry.cellWidth),0,geometry.columns-1);
    int row=std::max(0,(int)std::floor(relativeY/geometry.rowPitch));
    g_groupDropSlot = std::clamp(geometry.firstInGroup + row * geometry.columns + col, 0, (int)geometry.group->apps.size());
}

[[clang::no_destroy]] std::shared_ptr<Folder> g_groupHeaderPressed;
POINT g_groupHeaderStart{};
POINT g_groupHeaderAt{};
double g_groupHeaderEdgeTime=0;
int g_groupHeaderEdge=0;
bool GroupPointerDown(HWND hwnd, int x, int y) {
    if(g_groupEditMode)return FreeGroupPointerDown(hwnd,x,y);
    int index = GroupAtPoint(x, y, true);
    if (index < 0) return false;
    g_groupHeaderPressed = g_groupGeometry[index].group;
    g_groupHeaderStart = g_groupHeaderAt = {x,y}; SetCapture(hwnd); return true;
}
bool GroupPointerMove(int x, int y) {
    if(g_groupGesture.group)return FreeGroupPointerMove(x,y);
    if(!g_groupEditMode&&g_groupHeaderPressed)return true;
    if (!g_groupHeaderPressed) return false;
    if (std::abs(x-g_groupHeaderStart.x) > GetSystemMetrics(SM_CXDRAG) || std::abs(y-g_groupHeaderStart.y) > GetSystemMetrics(SM_CYDRAG))
        g_groupHeaderDragging = g_groupHeaderPressed;
    int index = GroupAtPoint(x,y);
    g_groupDropFolder = index < 0 ? nullptr : g_groupGeometry[index].group;
    g_groupHeaderAt={x,y};
    int edge=g_settings.pages ? (x<36*g_layout.scale?-1:x>g_layout.width-36*g_layout.scale?1:0) :
        (y<g_layout.viewTop?-1:y>=g_layout.viewBottom?1:0);
    if (edge!=g_groupHeaderEdge) {g_groupHeaderEdge=edge;g_groupHeaderEdgeTime=Now();}
    StartFrames(); return true;
}
void GroupHeaderTick() {
    if (g_groupHeaderEdge && Now()-g_groupHeaderEdgeTime>0.35) {
        g_groupHeaderEdgeTime=Now(); ScrollRows(g_groupHeaderEdge);
    }
    int index=GroupAtPoint(g_groupHeaderAt.x,g_groupHeaderAt.y);
    g_groupDropFolder=index<0?nullptr:g_groupGeometry[index].group;
}
void CancelGroupPointer() {
    bool preview=(bool)g_groupGesture.group;g_groupGesture={};
    if(preview){g_groupTransitions.clear();StartFrames();}
    g_groupHeaderPressed.reset(); g_groupHeaderDragging.reset(); g_groupDropFolder.reset();
    g_groupHeaderEdge=0;
}
bool GroupPointerUp(HWND hwnd) {
    if(g_groupEditMode)return FreeGroupPointerUp(hwnd);
    if (!g_groupHeaderPressed) return false;
    if (g_groupHeaderDragging && g_groupDropFolder && g_groupHeaderDragging != g_groupDropFolder) {
        MoveGroup(g_groupHeaderDragging, IndexOf(g_tiles, {nullptr,g_groupDropFolder}));
        SaveTiles();
    }
    CancelGroupPointer();
    if (GetCapture() == hwnd) ReleaseCapture();
    Refresh(); return true;
}

void RunBackupArchive(HWND owner, bool importing, const std::wstring& path);

void ImportOrRestoreLayout(HWND hwnd, bool restore) {
    std::wstring path = StoragePath(LayoutFile()) + L".bak";
    if (!restore) {
        WCHAR file[MAX_PATH]{};
        auto filter = std::wstring(Tr(L"Launchpad layout files")) + L'\0' + L"*.zip;*.txt;*.bak" + L'\0' + L'\0';
        OPENFILENAMEW dialog{sizeof(dialog)};
        dialog.hwndOwner=hwnd; dialog.lpstrFilter=filter.c_str(); dialog.lpstrFile=file; dialog.nMaxFile=ARRAYSIZE(file);
        dialog.lpstrTitle=Tr(L"Import layout file"); dialog.Flags=OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR;
        g_modal=true; BOOL selected=GetOpenFileNameW(&dialog); g_modal=false;
        if (!selected) return;
        path=file;
        if (EndsWith(ToLower(path), L".zip")) {
            RunBackupArchive(hwnd, true, path);
            return;
        }
    }
    auto records = ReadLines(path);
    std::wstring content;
    bool groupFile = !records.empty() && records[0][0] == L"schema";
    if (records.empty() || !ryjs_data::ReadUtf8(path, content) ||
        !ryjs_data::Validate(groupFile ? L"groups-v1-import.txt" : L"layout-import.txt", content) ||
        !ryjs_data::ValidateDirectory(DataRoot(), LayoutFile())) {
        g_modal=true;
        MessageBoxW(hwnd,Tr(L"The layout file is empty, unreadable, or uses a newer version."),Tr(L"Import layout file"),MB_OK|MB_ICONWARNING);
        g_modal=false; return;
    }
    std::vector<std::wstring> lines;
    for (const auto& fields : records) {
        std::wstring line;
        for (const auto& field : fields) { if (!line.empty()) line+=L'\t'; line+=field; }
        lines.push_back(std::move(line));
    }
    auto current=StoragePath(LayoutFile());
    if (GetFileAttributesW(current.c_str())!=INVALID_FILE_ATTRIBUTES &&
        !CopyFileW(current.c_str(),(current+L".before-import.bak").c_str(),FALSE)) {
        g_modal=true;
        MessageBoxW(hwnd,Tr(L"Could not back up the current layout. Import was cancelled."),Tr(L"Import layout file"),MB_OK|MB_ICONWARNING);
        g_modal=false; return;
    }
    auto previous = ArrangementLines();
    bool previousLayoutReadOnly = g_layoutReadOnly, previousDataReadOnly = g_dataReadOnly;
    g_openFolder=nullptr; g_query.clear(); g_caret=g_anchor=0;
    g_layoutReadOnly=g_dataReadOnly=false; ArrangeTiles(lines);
    if (!SaveTiles(true)) {
        ArrangeTiles(previous);
        g_layoutReadOnly=previousLayoutReadOnly; g_dataReadOnly=previousDataReadOnly;
        g_modal=true;
        MessageBoxW(hwnd,Tr(L"Could not save the layout. The previous layout was kept."),Tr(L"Import layout file"),MB_OK|MB_ICONWARNING);
        g_modal=false;
    }
    Refresh();
}

UINT_PTR CALLBACK LayoutExportHook(HWND dialog, UINT message, WPARAM, LPARAM data) {
    if (message == WM_NOTIFY) {
        auto notification = reinterpret_cast<const OFNOTIFYW*>(data);
        if (notification->hdr.code == CDN_TYPECHANGE)
            SendMessageW(GetParent(dialog), CDM_SETDEFEXT, 0,
                         reinterpret_cast<LPARAM>(notification->lpOFN->nFilterIndex == 2 ? L"txt" : L"zip"));
    }
    return 0;
}

void ExportLayout(HWND hwnd) {
    WCHAR file[32768] = L"ryjs-layout";
    auto filter = std::wstring(Tr(L"Layout, icons and settings (ZIP)")) + L'\0' + L"*.zip" + L'\0' +
                  Tr(L"Current layout only (TXT)") + L'\0' + L"*.txt" + L'\0' + L'\0';
    OPENFILENAMEW dialog{sizeof(dialog)};
    dialog.hwndOwner=hwnd; dialog.lpstrFilter=filter.c_str(); dialog.lpstrFile=file; dialog.nMaxFile=ARRAYSIZE(file);
    dialog.nFilterIndex=1; dialog.lpstrDefExt=L"zip"; dialog.lpstrTitle=Tr(L"Export layout file");
    dialog.Flags=OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_EXPLORER|OFN_ENABLEHOOK;
    dialog.lpfnHook=LayoutExportHook;
    g_modal=true; BOOL selected=GetSaveFileNameW(&dialog); g_modal=false;
    if (!selected) return;
    if (EndsWith(ToLower(file), L".zip")) {
        RunBackupArchive(hwnd, false, file);
        return;
    }
    if ((!g_layoutReadOnly && !g_dataReadOnly && !SaveTiles()) || g_layoutReadOnly || g_dataReadOnly) {
        MessageBoxW(hwnd,Tr(L"Could not export the layout. Use a full backup to preserve unreadable data."),Tr(L"Export layout file"),MB_OK|MB_ICONWARNING);
        return;
    }
    auto current=StoragePath(LayoutFile());
    if (_wcsicmp(file,current.c_str())==0 || !CopyFileW(current.c_str(),file,FALSE))
        MessageBoxW(hwnd,Tr(L"Could not save the layout. The previous layout was kept."),Tr(L"Export layout file"),MB_OK|MB_ICONWARNING);
}

#define SEAMLESS_PREVIEW 1
#include "../../mods/local/seamless-launchpad/seamless-launchpad.wh.cpp"
#include <cstdio>
#include <numeric>

namespace {
bool SavePng(IWICBitmapSource* bitmap, const std::wstring& path) {
    ComPtr<IWICImagingFactory> factory;
    ComPtr<IWICStream> stream;
    ComPtr<IWICBitmapEncoder> encoder;
    ComPtr<IWICBitmapFrameEncode> frame;
    ComPtr<IPropertyBag2> props;
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))) ||
        FAILED(factory->CreateStream(&stream)) ||
        FAILED(stream->InitializeFromFilename(path.c_str(), GENERIC_WRITE)) ||
        FAILED(factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder)) ||
        FAILED(encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache)) ||
        FAILED(encoder->CreateNewFrame(&frame, &props)) || FAILED(frame->Initialize(props.Get())) ||
        FAILED(frame->WriteSource(bitmap, nullptr)) || FAILED(frame->Commit()) || FAILED(encoder->Commit())) return false;
    return true;
}

bool SaveGrid(const std::wstring& path) {
    ComPtr<IWICImagingFactory> factory;
    ComPtr<IWICBitmap> bitmap;
    return SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))) &&
        SUCCEEDED(factory->CreateBitmapFromHBITMAP(g_surface.bitmap, nullptr, WICBitmapUsePremultipliedAlpha, &bitmap)) &&
        SavePng(bitmap.Get(), path);
}

bool SaveSettings(const std::wstring& path, int page, UINT dpi) {
    auto w = g_setWin;
    w->dpi = dpi;
    w->cw = 1100; w->ch = 860;
    w->page = page; w->pageT = 1;
    w->scrollY = w->scrollTarget = 0;
    Set_MakeFormats(*w);
    Set_BuildPage(*w);
    Set_FitBoxes(*w);
    Set_BuildPage(*w);
    ComPtr<IWICBitmap> bitmap;
    ComPtr<ID2D1RenderTarget> target;
    auto properties = D2D1::RenderTargetProperties();
    properties.dpiX = properties.dpiY = (float)dpi;
    float scale = dpi / 96.0f;
    return SUCCEEDED(g_setWic->CreateBitmap((UINT)(w->cw * scale), (UINT)((w->ch + kSetCapH) * scale), GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &bitmap)) &&
        SUCCEEDED(g_setD2d->CreateWicBitmapRenderTarget(bitmap.Get(), properties, &target)) &&
        (Set_Draw(*w, target.Get()), SavePng(bitmap.Get(), path));
}
} // namespace

void Preview_RunSmoke(HWND hwnd) {
    struct Size { int width, height; float scale; PCWSTR name; };
    bool ok = true;
    const auto initialLanguage = Preview::languageOverride;
    double enumerateStart = Now();
    g_settings.groups=false;
    OnAppsLoaded(EnumerateApps(IconPixels(), g_settings.source), AppsKey());
    double enumerateMs = (Now() - enumerateStart) * 1000;
    Preview::Log(L"smoke: discovered %zu actual apps in %.2f ms", g_apps.size(), enumerateMs);
    auto output = Preview::File(L"screenshots");
    SHCreateDirectoryExW(nullptr, output.c_str(), nullptr);
    FILE* csv = _wfopen(Preview::File(L"render-baseline.csv").c_str(), L"w");
    if (csv) fprintf(csv, "language,width,height,scale,samples,mean_ms,p95_ms,max_ms,mode\n");
    // Exercise the actual rendering paths without input injection or desktop automation.
    for (PCWSTR language : {L"zh-CN", L"en"}) {
        Preview::languageOverride = language;
        LoadSettings(); g_settings.groups=false;
        g_settings.opacity = 0.96f;
        g_settings.blurMode = BlurMode::None; // Reproducible, no desktop capture.
        g_labelFormat.Reset();
        g_canSleep = g_canHibernate = true;
        g_shown = true; g_appear = 1;
        UpdateFilter();
        for (auto size : {Size{1600,1000,1,L"desktop"}, Size{2560,1440,1.5f,L"2k"}, Size{3840,2160,1.5f,L"4k"}, Size{1080,1920,1.25f,L"portrait"}}) {
            g_layout.area = {0,0,size.width,size.height};
            g_layout.width = size.width; g_layout.height = size.height; g_layout.scale = size.scale;
            EnsureFormats(size.scale);
            ComputeLayout();
            Render();
            ok = SaveGrid(output + L"\\grid-" + language + L"-" + size.name + L".png") && ok;
        }
        // Baseline throughput is explicitly unpaced render time, not display FPS.
        // Match the phase-1 classic layout when comparing the two presenters.
        int savedHorizontal=g_settings.horizontalGap, savedVertical=g_settings.verticalGap;
        float savedLabelSize=g_settings.labelSize;
        g_settings.horizontalGap=64; g_settings.verticalGap=28; g_settings.labelSize=12.5f;
        g_labelFormat.Reset();
        g_layout.area = {0,0,1600,1000}; g_layout.width = 1600; g_layout.height = 1000; g_layout.scale = 1;
        EnsureFormats(1); ComputeLayout();
        std::vector<double> durations;
        for (int frame = 0; frame < 65; frame++) {
            double t = Now(); Render();
            if (frame >= 5) durations.push_back((Now()-t)*1000);
        }
        std::sort(durations.begin(), durations.end());
        Preview::Log(L"baseline %ls 1600x1000 render-only: mean=%.3f ms p95=%.3f ms max=%.3f ms, 60 samples; not presented FPS", language,
            std::accumulate(durations.begin(),durations.end(),0.0)/durations.size(), durations[56], durations.back());
        if (csv) fprintf(csv, "%s,1600,1000,1,60,%.6f,%.6f,%.6f,unpaced_render_only\n", wcscmp(language,L"en")==0 ? "en" : "zh-CN",
            std::accumulate(durations.begin(),durations.end(),0.0)/durations.size(), durations[56], durations.back());
        Preview::benchmarkGpu = true;
        durations.clear();
        for (int frame = 0; frame < 65; frame++) {
            double t = Now(); Render();
            if (frame >= 5) durations.push_back((Now()-t)*1000);
        }
        bool gpuAvailable = g_presenter.Active() && !g_presenter.failed;
        ok = gpuAvailable && ok;
        std::sort(durations.begin(), durations.end());
        Preview::Log(L"GPU composition %ls: available=%d mean=%.3f ms p95=%.3f ms max=%.3f ms; CPU submission cost, not physical presented FPS", language,
            gpuAvailable, std::accumulate(durations.begin(),durations.end(),0.0)/durations.size(), durations[56], durations.back());
        if (csv) fprintf(csv, "%s,1600,1000,1,60,%.6f,%.6f,%.6f,gpu_composition_cpu_submission\n", wcscmp(language,L"en")==0 ? "en" : "zh-CN",
            std::accumulate(durations.begin(),durations.end(),0.0)/durations.size(), durations[56], durations.back());
        Preview::benchmarkGpu = false;
        g_presenter.Reset();
        LaunchpadPresenter::LegacyStyle(hwnd);
        g_settings.horizontalGap=savedHorizontal; g_settings.verticalGap=savedVertical; g_settings.labelSize=savedLabelSize;
        g_labelFormat.Reset();
        if (!g_setWin && !Set_Create(true)) { ok = false; break; }
        Set_LoadFields(*g_setWin);
        for (int page = 0; page < kSetPageCount; page++)
            ok = SaveSettings(output + L"\\settings-" + language + L"-" + std::to_wstring(page) + L".png", page, 96) && ok;
        ok = SaveSettings(output + L"\\settings-" + language + L"-150pct.png", 2, 144) && ok;
    }
    if (csv) fclose(csv);
    if (g_frameWake) {
        UpdateFrameRate(ChosenMonitor());
        Preview::Log(L"Selected monitor refresh rate: %.6f Hz",g_refreshHz);
        double previous = Now();
        std::vector<double> ticks;
        g_refreshHz = 144;
        ArmNextFrame(true);
        for (int i = 0; i < 30; i++) {
            if (WaitForSingleObject(g_frameWake, 1000) != WAIT_OBJECT_0) { ok = false; break; }
            double now = Now(); ticks.push_back((now-previous)*1000); previous=now;
            ArmNextFrame(false);
        }
        CancelWaitableTimer(g_frameWake);
        double mean = ticks.empty() ? 0 : std::accumulate(ticks.begin(),ticks.end(),0.0)/ticks.size();
        Preview::Log(L"144 Hz timer scheduling check: samples=%zu mean interval=%.3f ms (not displayed FPS)",ticks.size(),mean);
        ok = ticks.size() == 30 && mean < 12 && ok;
    } else ok = false;
    // Search real application names and verify the retained filtering path.
    if (!g_apps.empty()) {
        g_query = g_apps[0]->name; UpdateFilter();
        ok = !g_visible.empty() && ok;
        Preview::Log(L"smoke: exact-name search returned %zu items", g_visible.size());
    } else ok = false;
    // Resource fallback and live language switching must not rewrite arbitrary input.
    launchpad_i18n::Select(L"zh-CN");
    ok = wcscmp(Tr(L"Save"), L"保存") == 0 && wcscmp(Tr(L"my-app-name"), L"my-app-name") == 0 && ok;
    launchpad_i18n::Select(L"en");
    ok = wcscmp(Tr(L"Save"), L"Save") == 0 && ok;
    // Persistence checks use a sandbox below preview-data, never user layouts.
    auto userDirectory = Preview::directory;
    Preview::directory += L"\\validation-state";
    SHCreateDirectoryExW(nullptr, Preview::directory.c_str(), nullptr);
    auto privateSettings=Preview::File(L"native-ui-settings.ini");
    ok=launchpad_settings_store::Save(privateSettings,{{L"uiFont",L"微软雅黑",L"auto"},{L"program",L"\"C:\\中文目录\\tool.exe\" --name=\"测试\"",L""}})&&ok;
    ok=launchpad_settings_store::Resolve(privateSettings,L"uiFont",L"auto")==L"微软雅黑"&&ok;
    ok=launchpad_settings_store::Resolve(privateSettings,L"program",L"")==L"\"C:\\中文目录\\tool.exe\" --name=\"测试\""&&ok;
    ok=launchpad_settings_store::Resolve(privateSettings,L"uiFont",L"Arial")==L"Arial"&&ok;
    ok=launchpad_settings_store::Resolve(privateSettings,L"uiFont",L"auto")==L"auto"&&ok;
    auto futureSettings=Preview::File(L"future-settings.ini");
    WritePrivateProfileStringW(L"meta",L"version",L"99",futureSettings.c_str());
    ok=!launchpad_settings_store::Save(futureSettings,{{L"uiFont",L"Arial",L"auto"}})&&ok;
    Preview::Log(L"smoke: native private settings Unicode/quotes roundtrip and later Windhawk per-key edits checked");
    if (g_apps.size() >= 2) {
        g_query.clear();
        ArrangeTiles({L"folder\t科研软件 · 中文重命名", L"in\t" + g_apps[0]->target, L"in\t" + g_apps[1]->target});
        auto before = ArrangementLines();
        SaveTiles();
        g_tiles.clear();
        LoadTiles();
        ok = before == ArrangementLines() && !g_tiles.empty() && g_tiles[0].folder &&
            g_tiles[0].folder->name == L"科研软件 · 中文重命名" && ok;
        // The settings UI's real Save path must clear the CLI language override.
        Preview::languageOverride = L"zh-CN";
        launchpad_i18n::Select(L"zh-CN");
        Set_LoadFields(*g_setWin);
        for (auto& field : g_setWin->fields)
            if (wcscmp(field.key, L"interfaceLanguage") == 0) field.cur = L"en";
        Set_Save(g_setWin);
        ok = !launchpad_i18n::chinese.load() && Preview::languageOverride.empty() && Preview::Setting(L"interfaceLanguage") == L"en" && ok;
        const auto& installedFonts = Set_InstalledFontFamilies();
        auto font = std::find(installedFonts.begin(), installedFonts.end(), L"Arial");
        if (font != installedFonts.end()) {
            for (auto& field : g_setWin->fields)
                if (wcscmp(field.key, L"uiFont") == 0) field.cur = *font;
            Set_Save(g_setWin);
            LoadSettings(); g_settings.groups=false; g_labelFormat.Reset(); EnsureFormats(1);
            WCHAR textFont[256], symbolFont[256];
            g_labelFormat->GetFontFamilyName(textFont, ARRAYSIZE(textFont));
            g_iconFormat->GetFontFamilyName(symbolFont, ARRAYSIZE(symbolFont));
            ok = g_settings.font == L"Arial" && wcscmp(textFont,L"Arial") == 0 && wcscmp(symbolFont,L"Arial") != 0 && ok;
        } else ok = false;
        Preview::PutSetting(L"uiFont", L"auto");
        Preview::Log(L"smoke: enumerated %zu installed font families; custom text font and separate symbol font checked", installedFonts.size());
        Preview::PutSetting(L"interfaceLanguage", L"zh-CN");
        LoadSettings(); g_settings.groups=false;
        ok = launchpad_i18n::chinese.load() && ok;
        g_settings.opacity = 0.96f; g_settings.blurMode = BlurMode::None;
        g_labelFormat.Reset(); EnsureFormats(1);
        g_openFolder = g_tiles[0].folder;
        g_folderAnim = 1; g_folderRenaming = true;
        g_caret = g_anchor = g_openFolder->name.size();
        UpdateFilter(); ComputeLayout(); Render();
        ok = SaveGrid(output + L"\\folder-zh-CN.png") && ok;
        g_openFolder = nullptr;
        Preview::Log(L"smoke: Unicode folder layout roundtrip and language settings Save path checked");
        // Exercise expanded groups using real installed apps in validation data.
        g_settings.groups = true; g_settings.pages = false; g_query.clear();
        std::vector<std::wstring> grouped{L"schema\t1\tgroups"};
        int app = 0;
        for (PCWSTR name : {L"科研软件",L"开发工具",L"办公软件",L"系统工具",L"娱乐软件"}) {
            grouped.push_back(std::wstring(L"folder\t")+name);
            for (int i = 0; i < 8 && app < (int)g_apps.size(); i++,app++) grouped.push_back(L"in\t"+g_apps[app]->target);
        }
        ArrangeTiles(grouped); UpdateFilter(); ComputeLayout();
        size_t applicationCount = g_visible.size();
        auto source = g_tiles[0].folder, destination = g_tiles[1].folder;
        Tile moved = source->apps[0];
        MoveAppToGroup(moved,destination,0);
        ok = destination->apps[0].app == moved.app && IndexOf(source->apps,moved)<0 && ok;
        SaveTiles();
        auto savedGroups = ArrangementLines();
        g_tiles.clear(); LoadTiles();
        ok = savedGroups == ArrangementLines() && ok;
        UpdateFilter(); ComputeLayout();
        // Real drag core: drop into another group, then cancel a second drag.
        g_pressed = IndexOf(g_visible,moved);
        auto cell = CellAt(g_pressed);
        g_drag.start={(LONG)cell.icon.left+2,(LONG)cell.icon.top+2};
        StartDrag();
        auto target = g_groupGeometry[2].title;
        DragMove((int)target.left+20,(int)target.top+12);
        EndDrag(true);
        ok = g_tiles[2].folder->apps[0].app == moved.app && ok;
        auto beforeCancel = ArrangementLines();
        g_pressed=IndexOf(g_visible,moved); cell=CellAt(g_pressed);
        g_drag.start={(LONG)cell.icon.left+2,(LONG)cell.icon.top+2};
        StartDrag(); EndDrag(false);
        ok = beforeCancel == ArrangementLines() && ok;
        // A still press must keep the scheduler alive until the hold threshold.
        g_pressed=IndexOf(g_visible,moved); cell=CellAt(g_pressed);
        g_drag.start={(LONG)cell.icon.left+2,(LONG)cell.icon.top+2};
        g_drag.pending=true; g_drag.pressTime=Now();
        g_appear=1; g_closing=false; StartFrames(); OnFrame();
        ok=g_drag.pending && !g_drag.active && g_frameTimer && ok;
        g_drag.pressTime=Now()-g_settings.holdMs/1000.0-0.1;
        OnFrame(); ok=g_drag.active && ok; EndDrag(false);
        // Temporarily missing applications must survive save, then rejoin the
        // original group and ordinal when application discovery sees them again.
        auto knownArrangement=ArrangementLines();
        auto removed=std::find(g_apps.begin(),g_apps.end(),moved.app);
        auto ordinal=std::distance(g_apps.begin(),removed);
        if (removed==g_apps.end()) ok=false;
        else {
            g_apps.erase(removed); ArrangeTiles(knownArrangement); SaveTiles();
            size_t missing=0;
            for (const auto& tile:g_tiles) if (tile.folder) missing+=tile.folder->unavailable.size();
            ok=missing==1 && knownArrangement==ArrangementLines() && ok;
            g_apps.insert(g_apps.begin()+ordinal,moved.app); LoadTiles();
            ok=knownArrangement==ArrangementLines() && ok;
            UpdateFilter(); ComputeLayout();
        }
        auto firstGroup=g_tiles[0].folder;
        MoveGroup(firstGroup,2);
        ok = g_tiles[2].folder == firstGroup && ok;
        auto empty = std::make_shared<decltype(source)::element_type>(); empty->name=L"空分组 · 可拖入应用";
        g_tiles.push_back({nullptr,empty});
        TidyTiles();
        ok = g_tiles.back().folder==empty && ok;
        UpdateFilter();
        ok = g_visible.size()==applicationCount && ok;
        g_tileMotion.clear(); g_settings.animations=false; g_requestedAnimations=false;
        for (auto size : {Size{1600,1000,1,L"desktop"}, Size{2560,1440,1.5f,L"2k"}, Size{3840,2160,1.5f,L"4k"}, Size{1080,1920,1.25f,L"portrait"}}) {
            g_layout.area={0,0,size.width,size.height}; g_layout.width=size.width; g_layout.height=size.height; g_layout.scale=size.scale;
            g_labelFormat.Reset(); EnsureFormats(size.scale); ComputeLayout();
            ok = g_groupCells.size()==g_visible.size() && ok;
            for (const auto& c : g_groupCells) ok = c.icon.right>c.icon.left && c.icon.left>=0 && c.icon.right<=size.width && ok;
            Render();
            ok=SaveGrid(output+L"\\groups-zh-CN-"+size.name+L".png")&&ok;
        }
        g_settings.pages=true; g_scroll=g_scrollTarget=0;
        ComputeLayout();
        ok=g_layout.pages>1 && ok;
        for (size_t i=0;i<g_groupCells.size();i++) {
            int page=(int)(g_groupCells[i].icon.left/g_layout.width);
            ok=page>=0 && page<g_layout.pages && ok;
            EnsureVisible((int)i);
            ok=g_scrollTarget>=0 && g_scrollTarget<=MaxScroll() && ok;
        }
        g_scroll=g_scrollTarget=0; Render();
        ok=SaveGrid(output+L"\\groups-zh-CN-pages.png")&&ok;
        // Free geometry and readability are rendered against synthetic backgrounds.
        // This never captures or changes the user's desktop layout.
        auto beforeGeometry=ArrangementLines();auto appearance=g_settings;
        g_settings.pages=false;g_layout.area={0,0,1600,1000};g_layout.width=1600;g_layout.height=1000;g_layout.scale=1;
        g_labelFormat.Reset();EnsureFormats(1);ComputeLayout();ToggleGroupEdit();
        int geometryIndex=0;
        for(const auto& tile:g_tiles)if(tile.folder) {
            int col=geometryIndex/2,row=geometryIndex%2;
            g_groupPositions[tile.folder]={24.0f+col*500.0f,row*340.0f,460,300};++geometryIndex;
        }
        g_freeGroupLayout=true;ok=SaveGroupPositions()&&beforeGeometry==ArrangementLines()&&ok;
        UpdateFilter();ComputeLayout();Render();ok=SaveGrid(output+L"\\free-group-edit-zh-CN.png")&&ok;
        g_groupEditMode=false;g_settings.blurMode=BlurMode::None;g_settings.opacity=1;g_settings.tint=D2D1::ColorF(0xFFFFFF);
        g_settings.textColor=D2D1::ColorF(0xFFFFFF);g_settings.labelPlateColor=D2D1::ColorF(0x111318);g_settings.labelPlateOpacity=0.65f;
        Render();ok=SaveGrid(output+L"\\readability-white-background.png")&&ok;
        g_settings.tint=D2D1::ColorF(0x111318);Render();ok=SaveGrid(output+L"\\readability-dark-background.png")&&ok;
        ok=beforeGeometry==ArrangementLines()&&ok;
        g_settings=appearance;ClearGroupPositions();DeleteFileW(GroupPositionFile().c_str());DeleteFileW((GroupPositionFile()+L".bak").c_str());
        UpdateFilter();ComputeLayout();
        Preview::Log(L"smoke: free group edit, independent geometry persistence and white/dark synthetic background readability rendered");
        SaveTiles(); auto supported=ArrangementLines();
        WriteLines(StoragePath(LayoutFile()),{L"schema\t999\tgroups",L"future\tkeep this file"});
        LoadTiles(); SaveTiles();
        auto future=ReadLines(StoragePath(LayoutFile()));
        ok=g_layoutReadOnly && !future.empty() && future[0][1]==L"999" && ok;
        ImportOrRestoreLayout(hwnd,true);
        ok=!g_layoutReadOnly && supported==ArrangementLines() && ok;
        Preview::Log(L"smoke: expanded group move, drag/cancel/hold, missing-app recovery, reorder, empty groups, Unicode persistence, four screen sizes, paging, backup restore and future-schema protection checked");
        g_settings.groups=false; g_settings.pages=false;
    } else ok = false;
    // Save tests queued reloads while validation storage was selected. They
    // must not run later against the user's independent preview data.
    MSG pending;
    while (PeekMessageW(&pending,hwnd,WM_APP_RELOAD,WM_APP_RELOAD,PM_REMOVE)) {}
    Preview::directory = userDirectory;
    Preview::languageOverride = initialLanguage;
    if (g_setWin) Settings_Close();
    Preview::Log(L"smoke result: %ls", ok ? L"PASS" : L"FAIL");
    Preview::smokePassed = ok;
    PostMessageW(hwnd, WM_APP_QUIT, 0, 0);
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    Preview::Init();
    int count = 0;
    auto argv = CommandLineToArgvW(GetCommandLineW(), &count);
    for (int i = 1; i < count; i++) {
        std::wstring arg = argv[i];
        if (arg == L"--settings") Preview::settingsOnStart = true;
        if (arg == L"--smoke") Preview::smoke = true;
        if (arg == L"--lang" && i+1 < count) Preview::languageOverride = argv[++i];
    }
    LocalFree(argv);
    HANDLE instance = nullptr;
    if (!Preview::smoke) {
        instance = CreateMutexW(nullptr, FALSE, L"Local\\RYJSLaunchpadPreview");
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            if (HWND window = FindWindowW(kWindowClass, nullptr))
                PostMessageW(window, Preview::settingsOnStart ? WM_APP + 20 : WM_APP + 61, 0, 0);
            if (instance) CloseHandle(instance);
            return 0;
        }
    }
    Preview::Log(L"standalone preview started; no Windhawk engine, keyboard hooks or Explorer injection");
    g_readyEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!g_readyEvent) return 2;
    StartThread(nullptr);
    CloseSettingsWindow();
    CloseHandle(g_readyEvent);
    if (instance) CloseHandle(instance);
    return Preview::smoke ? (Preview::smokePassed ? 0 : 1) : 0;
}

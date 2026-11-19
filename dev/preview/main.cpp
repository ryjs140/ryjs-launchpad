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

bool SaveSurface(HBITMAP bitmap, const std::wstring& path) {
    ComPtr<IWICImagingFactory> factory;
    ComPtr<IWICBitmap> wic;
    return SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))) &&
        SUCCEEDED(factory->CreateBitmapFromHBITMAP(bitmap, nullptr, WICBitmapUsePremultipliedAlpha, &wic)) &&
        SavePng(wic.Get(), path);
}

bool SaveGrid(const std::wstring& path) {
    return SaveSurface(g_surface.bitmap, path);
}

// Copies a 32-bit DIB section into memory. The surfaces D2D and the layered
// window share are premultiplied BGRA, bottom-up.
bool ReadDib(HBITMAP bitmap, int& width, int& height,
             std::vector<UINT32>& pixels) {
    if (!bitmap) return false;
    BITMAP info{};
    if (!GetObjectW(bitmap, sizeof(info), &info)) return false;
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = info.bmWidth;
    bmi.bmiHeader.biHeight = info.bmHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    pixels.assign((size_t)info.bmWidth * info.bmHeight, 0);
    HDC dc = GetDC(nullptr);
    int lines = GetDIBits(dc, bitmap, 0, (UINT)info.bmHeight, pixels.data(), &bmi,
                          DIB_RGB_COLORS);
    ReleaseDC(nullptr, dc);
    if (lines != info.bmHeight) return false;
    width = info.bmWidth;
    height = info.bmHeight;
    return true;
}

// Composites the menu surface over the grid, so the screenshot shows what the
// menu really looks like on the wallpaper instead of floating on white.
bool SaveOverlay(const std::wstring& path, HBITMAP base, HBITMAP overlay,
                 int ox, int oy) {
    int bw = 0, bh = 0, ow = 0, oh = 0;
    std::vector<UINT32> dst, src;
    if (!ReadDib(base, bw, bh, dst) || !ReadDib(overlay, ow, oh, src))
        return false;
    for (int y = 0; y < oh; y++) {
        int dy = bh - 1 - (oy + y);  // both DIBs are bottom-up
        if (dy < 0 || dy >= bh) continue;
        const int sy = oh - 1 - y;   // y counts from the top of the overlay
        for (int x = 0; x < ow; x++) {
            int dx = ox + x;
            if (dx < 0 || dx >= bw) continue;
            const UINT32 s = src[(size_t)sy * ow + x];
            const UINT32 a = s >> 24;
            if (!a) continue;
            UINT32& d = dst[(size_t)dy * bw + dx];
            const UINT32 inv = 255 - a;
            UINT32 out = 0xFF000000u;
            for (int shift = 0; shift < 24; shift += 8) {
                UINT32 c = ((s >> shift) & 0xFF) + ((((d >> shift) & 0xFF) * inv + 127) / 255);
                out |= (c > 255 ? 255 : c) << shift;
            }
            d = out;
        }
    }
    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = bw;
    bmi.bmiHeader.biHeight = bh;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    HDC dc = GetDC(nullptr);
    HBITMAP merged = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
    ReleaseDC(nullptr, dc);
    if (!merged) return false;
    memcpy(bits, dst.data(), dst.size() * sizeof(UINT32));
    bool ok = SaveSurface(merged, path);
    DeleteObject(merged);
    return ok;
}

// Renders the app menu offscreen over the grid that was just drawn, so the
// material can be checked without a window, input or a screen capture.
bool PreviewMenuShot(const std::wstring& path, int x, int y, int hover = -1,
                     int submenu = -1) {
    HMENU menu = CreatePopupMenu();
    HMENU groups = CreatePopupMenu();
    AppendMenuW(groups, MF_STRING, 1000, L"开发工具");
    AppendMenuW(groups, MF_STRING, 1001, L"远程");
    AppendMenuW(groups, MF_STRING, 1002, L"网盘与同步");
    AppendMenuW(menu, MF_STRING, 1, L"打开");
    AppendMenuW(menu, MF_POPUP, (UINT_PTR)groups, L"移动到分组");
    AppendMenuW(menu, MF_STRING, 91, L"从分组中移除");
    AppendMenuW(menu, MF_STRING, 95, L"重命名");
    AppendMenuW(menu, MF_STRING, 92, L"编辑条目…");
    AppendMenuW(menu, MF_STRING, 3, L"以管理员身份运行");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, 4, L"打开文件所在位置");
    AppendMenuW(menu, MF_STRING | MF_GRAYED, 5, L"打开快捷方式所在位置");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, 6, L"更改图标…");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, 7, L"从网格中移除");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, 8, L"属性");
    SetMenuDefaultItem(menu, 1, FALSE);
    LaunchpadMenu::g_previewScreen = g_surface.bitmap;
    LaunchpadMenu::g_previewScreenOrigin = {g_layout.area.left,
                                            g_layout.area.top};
    bool ok = PreviewRenderMenu(menu, x, y, TPM_LEFTALIGN | TPM_TOPALIGN,
                               hover, submenu) &&
              SaveOverlay(path, g_surface.bitmap,
                          LaunchpadMenu::g_menu.surface.bitmap,
                          LaunchpadMenu::g_menu.origin.x - g_layout.area.left,
                          LaunchpadMenu::g_menu.origin.y - g_layout.area.top);
    LaunchpadMenu::g_previewScreen = nullptr;
    DestroyMenu(menu);
    return ok;
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
        // The monitor dropdown lists the monitors connected right now: the
        // follow-the-mouse entry first, then one labeled entry per monitor.
        for (auto& f : g_setWin->fields) if (wcscmp(f.key, L"monitor") == 0) {
            int connected = 0;
            EnumDisplayMonitors(nullptr, nullptr,
                [](HMONITOR, HDC, LPRECT, LPARAM p) -> BOOL { (*(int*)p)++; return TRUE; },
                (LPARAM)&connected);
            bool listed = !f.options.empty() &&
                          wcscmp(f.options[0].first, L"mouse") == 0 &&
                          (int)f.options.size() == connected + 1 &&
                          f.options.back().first == std::to_wstring(connected);
            Preview::Log(L"smoke: monitor dropdown: %d connected display(s), %zu options: %ls",
                         connected, f.options.size(), listed ? L"PASS" : L"FAIL");
            for (auto& o : f.options)
                Preview::Log(L"smoke:   monitor option %ls = %ls", o.first, o.second);
            ok = listed && ok;
        }
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
    g_query.clear();
    g_caret=g_anchor=0;
    QueryChanged();
    const auto compactSearch=g_layout.search;
    // The field opens over a few frames now, so geometry is compared once the
    // animation has landed, and the first frame is checked for not jumping.
    auto settleSearch=[&]{
        for(int i=0;i<40&&g_searchExpand!=(SearchExpanded()?1.0f:0.0f);i++){
            g_lastFrame=Now()-.04;OnFrame();
        }
    };
    SendMessageW(hwnd,WM_CHAR,L'a',1);
    float beforeGrow=g_layout.search.right-g_layout.search.left;
    settleSearch();
    ok=g_query==L"a"&&beforeGrow<=compactSearch.right-compactSearch.left+.5f&&
       g_layout.search.right-g_layout.search.left>2*(compactSearch.right-compactSearch.left)&&ok;
    const auto typedSearch=g_layout.search;
    SendMessageW(hwnd,WM_KEYDOWN,VK_ESCAPE,1);
    settleSearch();
    ok=g_query.empty()&&g_layout.search.left==compactSearch.left&&g_layout.search.right==compactSearch.right&&ok;
    auto clickSearch=[&](D2D1_RECT_F rect) {
        auto point=MAKELPARAM((int)((rect.left+rect.right)/2),(int)((rect.top+rect.bottom)/2));
        SendMessageW(hwnd,WM_LBUTTONDOWN,MK_LBUTTON,point);
        SendMessageW(hwnd,WM_LBUTTONUP,0,point);
    };
    size_t unfiltered=g_visible.size();
    // The resting search icon is gone: at rest the grid starts above the
    // field's rect, and a click at the old pill spot no longer opens anything.
    ok=g_query.empty()&&!g_searchActivated&&
       g_layout.viewTop<g_layout.search.bottom&&ok;
    launchpad_i18n::Select(L"zh-CN");
    SendMessageW(hwnd,WM_CHAR,L'a',1);  // Typing is what opens the field now.
    settleSearch();
    Render();
    ok=SaveGrid(output+L"\\search-click-expanded-zh-CN.png")&&ok;
    launchpad_i18n::Select(L"en");
    SendMessageW(hwnd,WM_KEYDOWN,VK_ESCAPE,1);
    settleSearch();
    ok=!SearchExpanded()&&g_query.empty()&&ok;
    SendMessageW(hwnd,WM_CHAR,L'a',1);
    ok=g_query==L"a"&&g_caret==1&&ok;
    clickSearch(g_layout.clearButton);
    settleSearch();
    ok=g_searchActivated&&g_query.empty()&&g_visible.size()==unfiltered&&
        g_layout.search.left==typedSearch.left&&ok;
    SendMessageW(hwnd,WM_KEYDOWN,VK_ESCAPE,1);
    settleSearch();
    ok=!SearchExpanded()&&g_layout.search.left==compactSearch.left&&g_shown&&!g_closing&&ok;
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
        // A section is a caption and a run of icons, not a panel: the caption
        // keeps the click (it starts a drag), while the strip beside the icons
        // is empty space the grid closes on.
        if (!g_groupGeometry.empty()) {
            const auto& group = *g_groupGeometry.begin();
            auto visual = VisualGroupRect(group), title = ProjectGroupRect(group.title);
            int captionX = (int)((title.left + title.right) / 2);
            int captionY = (int)((title.top + title.bottom) / 2);
            int emptyX = (int)(visual.left + 3);
            int emptyY = (int)std::min(title.bottom + 2, visual.bottom - 2);
            ok = GroupAtPoint(captionX, captionY, true) == 0 &&
                 ClickKeepsOpen(captionX, captionY) && GroupAtPoint(emptyX, emptyY) == 0 &&
                 GroupAtPoint(emptyX, emptyY, true) < 0 &&
                 !ClickKeepsOpen(emptyX, emptyY) && ok;
        }
        // Sample the production renderer in both directions at identical phases.
        auto animationLayout = g_layout;
        bool priorFlyIn = g_settings.iconFlyIn;
        g_settings.iconFlyIn = true;
        for (auto size : {Size{1600,1000,1,L"wide"}, Size{1080,1920,1.25f,L"portrait"}}) {
            g_layout.area = {0,0,size.width,size.height};
            g_layout.width = size.width; g_layout.height = size.height; g_layout.scale = size.scale;
            EnsureFormats(size.scale); ComputeLayout();
            auto prefix = output + L"\\fly-in-" + size.name;
            for (int frame : {0,18,32,52,75,100}) {
                g_appear = frame / 100.f;
                Render();
                ok = SaveGrid(prefix + L"-" + std::to_wstring(frame) + L".png") && ok;
            }
            g_appear = .32f; g_closing = true;
            Render();
            ok = SaveGrid(prefix + L"-32-reverse.png") && ok;
            g_closing = false;
            g_appear = 1; g_settings.iconFlyIn = false;
            Render();
            ok = SaveGrid(prefix + L"-classic-100.png") && ok;
            g_settings.iconFlyIn = true;
        }
        // Exercise actual toggle entry points while the canvas is still hidden.
        bool requestedAnimations = g_requestedAnimations.exchange(true);
        bool systemAnimations = g_systemAnimations.exchange(true);
        g_appear = .5f; g_appearVelocity = 1000.0 / g_settings.openMs;
        Toggle();
        bool reverses = g_closing && g_appear == .5f && g_appearVelocity > 0;
        g_lastFrame = Now() - .04;
        OnFrame();
        reverses = reverses && g_closing && g_shown && g_appear < .5f && g_appearVelocity < 0;
        float closingPhase = g_appear;
        Toggle();
        reverses = reverses && !g_closing && g_shown && g_appear == closingPhase &&
                   g_appearVelocity < 0;
        g_lastFrame = Now() - .04;
        OnFrame();
        reverses = reverses && g_appear > closingPhase && g_appearVelocity > 0;
        g_lastFrame = Now() - 2;
        OnFrame();
        ok = reverses && g_appear == 1 && g_appearVelocity == 0 && ok;
        // The Windows key itself decides the toggle: once the mod answered the
        // first show, Windows stops repeating that request, so the second tap
        // arrives only as a tap. The show request that accompanies the same
        // keystroke must not toggle a second time.
        g_keyToggleAt = -1000;
        SendMessageW(hwnd, WM_APP_WIN_TAP, 0, 0);
        bool tapCloses = g_closing && g_shown;
        SendMessageW(hwnd, WM_APP_TOGGLE, 1, 0);
        bool oneToggle = g_closing && g_shown;
        g_closing = false; g_appear = 1; g_appearVelocity = 0;
        ok = tapCloses && oneToggle && ok;
        Preview::Log(L"smoke: lone Windows key tap closes, duplicate show request ignored: %ls",
                     tapCloses && oneToggle ? L"PASS" : L"FAIL");
        g_requestedAnimations = requestedAnimations; g_systemAnimations = systemAnimations;
        g_settings.iconFlyIn = priorFlyIn;
        g_layout = animationLayout; EnsureFormats(g_layout.scale); ComputeLayout();
        Preview::Log(L"smoke: grouped fly-in frames in wide and portrait, classic settled frame, real Toggle reversal: %ls", reverses ? L"PASS" : L"FAIL");
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
            GroupPosition position;
            position.style=5;position.gridW=6;position.gridH=10;
            g_groupPositions[tile.folder]=MoveGroupToGrid(position,col*7,row*12);
            ++geometryIndex;
        }
        g_freeGroupLayout=true;ok=SaveGroupPositions(true)&&beforeGeometry==ArrangementLines()&&ok;
        UpdateFilter();ComputeLayout();Render();ok=SaveGrid(output+L"\\free-group-edit-zh-CN.png")&&ok;
        // Render the resize preview with its independent whole-cell target.
        if(!g_groupGeometry.empty()) {
            const auto& first=g_groupGeometry.front();
            auto handle=GroupResizeHit(first.area);
            FreeGroupPointerDown(hwnd,(int)(handle.left+10),(int)(handle.top+10));
            if(g_groupGesture.group) {
                FreeGroupPointerMove(g_groupGesture.start.x-38,g_groupGesture.start.y+34);
                Render();ok=SaveGrid(output+L"\\whole-cell-resize-zh-CN.png")&&ok;
                CancelGroupPointer();
                if(GetCapture()==hwnd)ReleaseCapture();
            } else ok=false;
        }
        g_groupEditMode=false;g_settings.powerBar=true;ComputeLayout();
        std::vector<int> selectable;
        for(const auto& geometry:g_groupGeometry)if(geometry.count>0&&
            g_visible[geometry.start].app&&g_groupCells[geometry.start].icon.top<g_layout.viewBottom)
            selectable.push_back(geometry.start);
        if(selectable.size()>=2) {
            const auto originalTint=g_settings.tint;
            const float originalOpacity=g_settings.opacity;
            g_settings.tint=D2D1::ColorF(0x343434);g_settings.opacity=1;
            SendMessageW(hwnd,WM_MOUSELEAVE,0,0);
            Render();
            auto unit=CellAt(selectable[0]).unit;
            auto sample=[&](D2D1_RECT_F box,float u,float v) {
                return POINT{(LONG)std::lround(std::lerp(box.left,box.right,u)),
                             (LONG)std::lround(std::lerp(box.top,box.bottom,v))};
            };
            auto pixel=[&](POINT p) {return (int)GetRValue(GetPixel(g_surface.dc,p.x,p.y));};
            auto next=sample(CellAt(selectable[1]).unit,.05f,.10f);
            int neighbour=pixel(next);
            auto left=sample(unit,.009f,.22f),right=sample(unit,.991f,.22f);
            auto top=sample(unit,.22f,.009f),bottom=sample(unit,.22f,.991f);
            auto move=[&](POINT p) {SendMessageW(hwnd,WM_MOUSEMOVE,0,MAKELPARAM(p.x,p.y));Render();};
            std::array<POINT,4> catches{left,right,top,bottom};
            for(size_t edge=0;edge<catches.size();++edge) {
                move(catches[edge]);
                ok=g_glassClaimed&&pixel(catches[edge])>pixel(catches[edge^1])+22&&pixel(next)==neighbour&&ok;
            }
            for(float x:{.012f,.988f})for(float y:{.012f,.988f}) {
                move(sample(unit,x,y));
                float edgeX=x<.5f?.009f:.991f, edgeY=y<.5f?.009f:.991f;
                float alongX=x<.5f?.08f:.92f, alongY=y<.5f?.08f:.92f;
                ok=g_glassClaimed&&pixel(sample(unit,edgeX,alongY))>100&&
                    pixel(sample(unit,alongX,edgeY))>100&&ok;
            }
            ok=SaveGrid(output+L"\\glass-corner-zh-CN.png")&&ok;
            move(left);ok=SaveGrid(output+L"\\square-grid-hover-zh-CN.png")&&ok;

            double started=Now();
            for(int frame=0;frame<24;++frame) {
                auto slot=CellAt(selectable[frame%2]).unit;
                move(sample(slot,.12f+(frame%6)*.14f,.20f+(frame%5)*.13f));
                ok=g_glassClaimed&&ok;
            }
            Preview::Log(L"tile glass: 24 alternating-tile pointer messages and frames, mean %.3f ms (smoke harness, not displayed FPS)",
                         (Now()-started)*1000/24);
            SendMessageW(hwnd,WM_MOUSELEAVE,0,0);Render();
            ok=!g_glassClaimed&&pixel(next)==neighbour&&ok;
            int iconSize=g_settings.iconSize;
            float labelSize=g_settings.labelSize;
            for(float scale:{1.25f,1.5f}) {
                g_layout.scale=scale;
                g_settings.iconSize=iconSize+12;
                g_settings.labelSize=labelSize+3;
                EnsureFormats(scale);
                ComputeLayout();
                g_scroll=g_scrollTarget=std::min(36*scale,MaxScroll());
                auto cell=CellAt(selectable[0]);
                move(sample(cell.unit,.02f,.28f));
                auto fade=std::find_if(std::begin(g_glowFades),std::end(g_glowFades),[&](const auto& slot) {
                    return slot.key==g_visible[selectable[0]].app.get()&&slot.alpha>0;
                });
                ok=g_glassClaimed&&fade!=std::end(g_glowFades)&&std::abs(fade->light.x-.02f)<.012f&&
                    std::abs(fade->light.y-.28f)<.012f&&InRect(cell.unit,cell.icon.left,cell.icon.top)&&ok;
            }
            SendMessageW(hwnd,WM_MOUSELEAVE,0,0);
            g_settings.iconSize=iconSize;g_settings.labelSize=labelSize;
            g_layout.scale=1;g_scroll=g_scrollTarget=0;
            EnsureFormats(1);ComputeLayout();
            g_settings.tint=originalTint;g_settings.opacity=originalOpacity;
            auto one=CellAt(selectable[0]).highlight,two=CellAt(selectable[1]).highlight;
            g_selectionPointer.pending=true;
            g_selectionPointer.start={(LONG)std::min(one.left,two.left)-5,(LONG)std::min(one.top,two.top)-5};
            MoveAppSelection((int)std::max(one.right,two.right)+5,(int)std::max(one.bottom,two.bottom)+5);
            ok=AppSelected(*g_visible[selectable[0]].app)&&AppSelected(*g_visible[selectable[1]].app)&&ok;
            Render();ok=SaveGrid(output+L"\\batch-selection-zh-CN.png")&&ok;
            ClearAppSelection();
        } else ok=false;
        g_groupEditMode=false;g_settings.blurMode=BlurMode::None;g_settings.opacity=1;g_settings.tint=D2D1::ColorF(0xFFFFFF);
        Render();ok=SaveGrid(output+L"\\readability-white-background.png")&&ok;
        g_settings.tint=D2D1::ColorF(0x111318);Render();ok=SaveGrid(output+L"\\readability-dark-background.png")&&ok;
        g_settings.tint=D2D1::ColorF(0x808080);Render();ok=SaveGrid(output+L"\\readability-gray-background.png")&&ok;
        // Exercise the common white labels over a synthetic split-luminance background.
        std::vector<UINT32> mixed(160*100);
        for(int y=0;y<100;y++)for(int x=0;x<160;x++)mixed[y*160+x]=x<80?0xFFF5F5F5:0xFF090909;
        ComPtr<ID2D1Bitmap> mixedBitmap;
        auto mixedProps=D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),96,96);
        ok=SUCCEEDED(g_target->CreateBitmap(D2D1::SizeU(160,100),mixed.data(),160*4,mixedProps,&mixedBitmap))&&ok;
        g_backdropImage.bitmap=mixedBitmap;g_backdropImage.sourceWidth=160;g_backdropImage.sourceHeight=100;
        g_settings.blurMode=BlurMode::Custom;g_settings.opacity=0;g_settings.backgroundDim=0;
        Render();ok=SaveGrid(output+L"\\readability-mixed-background.png")&&ok;
        for(float progress:{0.0f,.25f,.5f,.75f,1.0f}){g_appear=progress;Render();}
        g_closing=true;for(float progress:{.75f,.5f,.25f}){g_appear=progress;Render();}g_closing=false;g_appear=1;

        // The app menu: laid out and painted offscreen over the grid just
        // drawn, so its material is checked without a window or input. The
        // menu base now comes from the grid's own frame, so the synthetic
        // backdrop stays: it is what makes the frame opaque here.
        g_settings.blurMode=BlurMode::Custom;g_settings.opacity=0.35f;g_settings.tint=D2D1::ColorF(0x11111B);
        Render();
        ok=PreviewMenuShot(output+L"\\menu-tile-zh-CN.png",g_layout.area.left+180,g_layout.area.top+140)&&ok;
        Render();
        ok=PreviewMenuShot(output+L"\\menu-hover-zh-CN.png",g_layout.area.left+180,g_layout.area.top+140,2)&&ok;
        Render();
        ok=PreviewMenuShot(output+L"\\menu-submenu-zh-CN.png",g_layout.area.left+180,g_layout.area.top+140,1,1)&&ok;
        g_backdropImage={};
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
    Preview::uiCheck=GetFileAttributesW(Preview::File(L"ui-check.flag").c_str())!=INVALID_FILE_ATTRIBUTES;
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

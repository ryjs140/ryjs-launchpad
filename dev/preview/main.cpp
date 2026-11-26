#define SEAMLESS_PREVIEW 1
#include "../../mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp"
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
    AppendMenuW(menu, MF_STRING, 1, L"打开\tEnter");
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
    bool ok = PreviewRenderMenu(menu, x, y, TPM_LEFTALIGN | TPM_TOPALIGN,
                               hover, submenu) &&
              LaunchpadMenu::g_menu.root.front().label.shadow.bitmap &&
              LaunchpadMenu::g_menu.root.front().shortcut.shadow.bitmap &&
              SaveOverlay(path, g_surface.bitmap,
                          LaunchpadMenu::g_menu.surface.bitmap,
                          LaunchpadMenu::g_menu.origin.x - g_layout.area.left,
                          LaunchpadMenu::g_menu.origin.y - g_layout.area.top);
    auto& state = LaunchpadMenu::g_menu;
    auto panelFill = [&](const LaunchpadMenu::Panel& panel) {
        return GetPixel(state.surface.dc,
                        (int)((panel.rect.left + panel.rect.right) / 2) - state.origin.x,
                        (int)(panel.rect.top + 4 * state.scale) - state.origin.y);
    };
    if (ok && submenu >= 0) {
        ok = state.panels.size() > 1 && panelFill(state.panels[0]) != CLR_INVALID &&
             panelFill(state.panels[0]) == panelFill(state.panels[1]);
    }
    DestroyMenu(menu);
    return ok;
}

bool SaveSettings(const std::wstring& path, int page, UINT dpi, float scroll = 0) {
    auto w = g_setWin;
    w->dpi = dpi;
    w->cw = 1100; w->ch = 860;
    w->page = page; w->pageT = 1;
    w->scrollY = w->scrollTarget = scroll;
    Set_MakeFormats(*w);
    Set_BuildPage(*w);
    Set_FitBoxes(*w);
    Set_BuildPage(*w);
    // The smoke copies the live window, whose toggle knobs may still be at an
    // animated midpoint from an earlier interaction. A still frame must show
    // the field's actual state, not the transient animation value.
    w->knob.clear();
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
        g_settings.backend = launchpad_material::Backend::Transparent; // Reproducible, no desktop capture.
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
                          wcscmp(f.options[0].first.c_str(), L"mouse") == 0 &&
                          (int)f.options.size() >= connected + 1;
            Preview::Log(L"smoke: monitor dropdown: %d connected display(s), %zu options: %ls",
                         connected, f.options.size(), listed ? L"PASS" : L"FAIL");
            for (auto& o : f.options)
                Preview::Log(L"smoke:   monitor option %ls", o.second.c_str());
            ok = listed && ok;
        }
        for (int page = 0; page < kSetPageCount; page++)
            ok = SaveSettings(output + L"\\settings-" + language + L"-" + std::to_wstring(page) + L".png", page, 96) && ok;
        ok = SaveSettings(output + L"\\settings-" + language + L"-150pct.png", 2, 144) && ok;
        if (wcscmp(language, L"zh-CN") == 0) {
            // The earlier control tests leave some switches flipped in the
            // shared window; this evidence frame shows the documented default.
            if (auto* f = Set_FieldByKey(*g_setWin, L"powerWarning")) {
                Set_PutValue(*f, L"1");
                f->value = L"1";
            }
            ok = SaveSettings(output + L"\\settings-zh-CN-3-warning.png", 3, 96, 700) && ok;
        }        if (wcscmp(language, L"zh-CN") == 0)
            ok = SaveSettings(output + L"\\settings-zh-CN-2-material-buttons.png", 2, 96, 1420) && ok;
        if (wcscmp(language, L"zh-CN") == 0) {
            // Both palettes, forced, so the dark look cannot regress silently
            // just because this machine's Windows app theme happens to be light.
            const std::wstring previousTheme = GetStringSetting(L"themeMode");
            for (const wchar_t* theme : {L"light", L"dark"}) {
                Preview::PutSetting(L"themeMode", theme);
                settings_palette::Refresh();
                Set_LoadFields(*g_setWin);  // The dropdown must match the palette.
                for (int page = 0; page < kSetPageCount; page++)
                    ok = SaveSettings(output + L"\\settings-" + theme + L"-zh-CN-" +
                                          std::to_wstring(page) + L".png", page, 96) && ok;
                ok = SaveSettings(output + L"\\settings-" + theme + L"-zh-CN-150pct.png", 2,
                                  144) && ok;
            }
            Preview::PutSetting(L"themeMode", previousTheme);
            settings_palette::Refresh();
        }
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
    launchpad_i18n::Select(L"zh-CN");
    SendMessageW(hwnd, WM_IME_STARTCOMPOSITION, 0, 0);
    ok = g_imeComposition.active && SearchExpanded() && g_searchExpand == 1.f &&
        g_visible.size() == unfiltered && g_query.empty() && ok;
    g_imeComposition.text = L"zhong wen";
    g_imeComposition.cursor = 5;
    const auto inlineIme = GetEditView(false);
    IMECHARPOSITION imePosition{sizeof(imePosition), 5};
    ok = inlineIme.compositionLength == 9 && inlineIme.caret == 5 &&
        SendMessageW(hwnd, WM_IME_REQUEST, IMR_QUERYCHARPOSITION, (LPARAM)&imePosition) &&
        imePosition.pt.x >= imePosition.rcDocument.left &&
        imePosition.pt.x <= imePosition.rcDocument.right && ok;
    Render();
    ok = SaveGrid(output + L"\\search-ime-composition-zh-CN.png") && ok;
    SendMessageW(hwnd, WM_IME_ENDCOMPOSITION, 0, 0);
    ok = !g_imeComposition.active && g_imeComposition.text.empty() &&
        g_query.empty() && g_visible.size() == unfiltered && ok;
    SendMessageW(hwnd, WM_KEYDOWN, VK_ESCAPE, 1);
    settleSearch();
    launchpad_i18n::Select(L"en");
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
    ok = launchpad_settings_store::Save(privateSettings, {{L"glowDistance", L"80", L""}}) && ok;
    const auto declaredReach = DeclaredSettingDefault(L"glowDistance");
    const bool seededDefaultPreserved = declaredReach == L"24" &&
        launchpad_settings_store::Resolve(privateSettings, L"glowDistance", L"24", declaredReach) == L"80" &&
        launchpad_settings_store::Read(privateSettings, L"present", L"glowDistance") == L"1" &&
        launchpad_settings_store::Resolve(privateSettings, L"glowDistance", L"40", declaredReach) == L"40" &&
        launchpad_settings_store::Read(privateSettings, L"present", L"glowDistance").empty();
    ok = seededDefaultPreserved && ok;
    Preview::Log(L"smoke: Windhawk-seeded defaults preserve private settings: %ls",
                 seededDefaultPreserved ? L"PASS" : L"FAIL");
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
            // DirectWrite can report a substituted family name for a requested font.
            ComPtr<IDWriteTextFormat> expected;
            WCHAR expectedFont[256]{}, textFont[256]{}, symbolFont[256]{};
            ok = g_settings.font == *font &&
                SUCCEEDED(g_dwrite->CreateTextFormat(font->c_str(), nullptr, DWRITE_FONT_WEIGHT_SEMI_BOLD,
                    DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.f, L"", &expected)) &&
                SUCCEEDED(expected->GetFontFamilyName(expectedFont, ARRAYSIZE(expectedFont))) &&
                SUCCEEDED(g_labelFormat->GetFontFamilyName(textFont, ARRAYSIZE(textFont))) &&
                SUCCEEDED(g_iconFormat->GetFontFamilyName(symbolFont, ARRAYSIZE(symbolFont))) &&
                wcscmp(textFont, expectedFont) == 0 && wcscmp(symbolFont, expectedFont) != 0 && ok;
        } else ok = false;
        Preview::PutSetting(L"uiFont", L"auto");
        Preview::Log(L"smoke: enumerated %zu installed font families; custom text font and separate symbol font checked", installedFonts.size());
        {
            auto& settings = *g_setWin;
            const auto oldPowerOrder = Preview::Setting(L"powerOrder");
            auto orderField = Set_PowerOrderField(settings);
            orderField->cur = orderField->def;
            Set_GoPage(settings, 3);
            auto first = std::find_if(settings.items.begin(), settings.items.end(), [](const SetItem& row) {
                return row.powerPosition == 0;
            });
            const int lockField = first->field;
            const auto arrows = Set_Parts(settings.fields[lockField], Set_RowRect(settings, *first));
            auto center = [&](const D2D1_RECT_F& box) {
                const float scale = settings.dpi / 96.f;
                return MAKELPARAM(std::lround((box.left + box.right) / 2 * scale),
                    std::lround(((box.top + box.bottom) / 2 - settings.scrollY + kSetCapH) * scale));
            };
            const auto enabled = settings.fields[lockField].cur;
            SendMessageW(settings.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, center(arrows.powerMove[0]));
            bool orderOk = ParsePowerOrder(orderField->cur) == DefaultPowerOrder() &&
                           settings.fields[lockField].cur == enabled;
            SendMessageW(settings.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, center(arrows.powerMove[1]));
            const PowerOrder expected{1, 0, 2, 3, 4, 5};
            const auto* moved = Set_ItemOfField(settings, lockField);
            orderOk = orderOk && moved && moved->powerPosition == 1 &&
                      settings.fields[lockField].cur == enabled && ParsePowerOrder(orderField->cur) == expected;
            orderOk = ParsePowerOrder(L" restart,unknown,restart,lock ") == PowerOrder{4, 0, 1, 2, 3, 5} && orderOk;
            Set_Save(g_setWin);
            LoadSettings();
            orderOk = orderOk && g_settings.powerOrder == expected &&
                      Preview::Setting(L"powerOrder") == FormatPowerOrder(expected);
            const bool hibernate = std::exchange(g_canHibernate, false);
            g_settings.powerShown[0] = false;
            ComputeLayout();
            orderOk = orderOk && !g_layout.powerItems.empty() && g_layout.powerItems.front() == 1 &&
                      std::find(g_layout.powerItems.begin(), g_layout.powerItems.end(), 0) == g_layout.powerItems.end() &&
                      std::find(g_layout.powerItems.begin(), g_layout.powerItems.end(), 3) == g_layout.powerItems.end();
            g_canHibernate = hibernate;
            Set_ResetToSuggested(settings);
            orderOk = orderOk && ParsePowerOrder(orderField->cur) == DefaultPowerOrder() &&
                      Set_ItemOfField(settings, lockField)->powerPosition == 0;
            launchpad_i18n::Select(L"zh-CN");
            orderField->cur = FormatPowerOrder(expected);
            orderOk = SaveSettings(output + L"\\settings-power-order-zh-CN.png", 3, 96) && orderOk;
            Preview::PutSetting(L"powerOrder", oldPowerOrder);
            Set_LoadFields(settings);
            Set_BuildPage(settings);
            LoadSettings();
            Preview::Log(L"smoke: power-order controls, persistence, defaults and filtering: %ls",
                         orderOk ? L"PASS" : L"FAIL");
            ok = orderOk && ok;
        }
        {
            // Saving, deleting, then round-tripping a material file through the
            // same helpers the buttons call. The dialogs themselves are not
            // exercised; the document layer is.
            auto& settings = *g_setWin;
            const auto oldPresets = Preview::Setting(L"materialUserPresets");
            Preview::PutSetting(L"materialUserPresets", L"");
            Set_GoPage(settings, 2);
            settings.dpi = 96;  // A smoke-tested window is invisible, so it reports no DPI.
            Set_MakeFormats(settings);
            Set_LoadFields(settings);
            Set_BuildPage(settings);
            const auto actions = std::find_if(settings.items.begin(), settings.items.end(),
                [](const SetItem& row) { return row.type == SetItem::Actions; });
            bool presetOk = actions != settings.items.end() &&
                            settings.userPresets.empty() &&
                            Set_FieldByKey(settings, L"materialPreset")->options.size() == 11;
            settings.scrollY = settings.scrollTarget =
                std::clamp(actions != settings.items.end() ? actions->y - 120 : 0.0f,
                           0.0f, std::max(0.0f, settings.contentH - Set_ViewH(settings)));
            settings.dpi = 96;  // Set by the click path; LoadFields resets it for an invisible window.
            if (actions != settings.items.end()) {
                const D2D1_RECT_F first = Set_ActionRect(settings, *actions, 0);
                presetOk = presetOk && Set_Hit(settings, (first.left + first.right) / 2,
                                               (first.top + first.bottom) / 2 - settings.scrollY).kind ==
                                           SetHit::Action;
                Set_PutValue(*Set_FieldByKey(settings, L"materialPresetName"), L"Smoke A");
                Set_MaterialAction(settings, 0);  // The button rect and hit mapping are asserted above.
                std::vector<launchpad_material::UserPreset> stored;
                // The preview settings file is a plain INI and cannot hold the
                // multi-line JSON, so the stored form is checked in memory; the
                // real settings store encodes it per line.
                const bool storedParsed = launchpad_material::ParseUserPresets(
                    launchpad_material::UserPresetsJson(settings.userPresets), stored);
                presetOk = presetOk && settings.userPresets.size() == 1 &&
                           settings.userPresets[0].name == L"Smoke A" &&
                           Set_FieldByKey(settings, L"materialPreset")->cur == L"Smoke A" &&
                           Set_FieldByKey(settings, L"materialPreset")->options.size() == 12;
                presetOk = presetOk && storedParsed &&
                    stored.size() == 1 && stored[0].name == L"Smoke A";
                const auto savedParams = Set_MaterialParams(settings);
                Set_PutValue(*Set_FieldByKey(settings, L"tintColor"), L"#89B4FA");
                Set_PutValue(*Set_FieldByKey(settings, L"materialBlur"), L"77");
                Set_ApplyMaterialPreset(settings, L"Smoke A");
                const auto picked = Set_MaterialParams(settings);
                const std::wstring pickedTint = Set_FieldByKey(settings, L"tintColor")->cur;
                presetOk = presetOk &&
                    std::abs(picked.blurSigma - savedParams.blurSigma) < 0.01f &&
                    std::abs(picked.saturation - savedParams.saturation) < 0.01f &&
                    std::abs(picked.brightness - savedParams.brightness) < 0.01f &&
                    std::abs(picked.contrast - savedParams.contrast) < 0.01f &&
                    std::abs(picked.tintOpacity - savedParams.tintOpacity) < 0.01f &&
                    std::abs(picked.luminosity - savedParams.luminosity) < 0.01f &&
                    std::abs(picked.noise - savedParams.noise) < 0.01f &&
                    std::abs(picked.dim - savedParams.dim) < 0.01f &&
                    pickedTint == L"#89B4FA" &&
                    launchpad_material::RgbFromColor(picked.tint) == 0x89B4FA;
                Set_ApplyMaterialPreset(settings, L"Vivid Glass");
                presetOk = presetOk &&
                    Set_FieldByKey(settings, L"tintColor")->cur == L"#89B4FA";
                Set_MaterialAction(settings, 1);
                presetOk = presetOk && settings.userPresets.empty() &&
                           Set_FieldByKey(settings, L"materialPreset")->cur == L"custom" &&
                           Set_FieldByKey(settings, L"materialPreset")->options.size() == 11;
            }
            const auto before = Set_MaterialParams(settings);
            const std::wstring doc = Set_MaterialDocument(settings, L"Round trip");
            Set_ApplyMaterialParams(settings,
                launchpad_material::ParamsFromPreset(launchpad_material::Presets()[3]));
            const bool restored = Set_ApplyMaterialDocument(settings, doc) == nullptr;
            const auto after = Set_MaterialParams(settings);
            const bool same = std::abs(after.blurSigma - before.blurSigma) < 0.01f &&
                std::abs(after.saturation - before.saturation) < 0.01f &&
                std::abs(after.brightness - before.brightness) < 0.01f &&
                std::abs(after.contrast - before.contrast) < 0.01f &&
                std::abs(after.tintOpacity - before.tintOpacity) < 0.01f &&
                std::abs(after.luminosity - before.luminosity) < 0.01f &&
                std::abs(after.noise - before.noise) < 0.01f &&
                std::abs(after.dim - before.dim) < 0.01f &&
                launchpad_material::RgbFromColor(after.tint) ==
                    launchpad_material::RgbFromColor(before.tint);
            const wchar_t* garbage = Set_ApplyMaterialDocument(settings, L"{}");
            presetOk = presetOk && restored && same && garbage &&
                        wcscmp(garbage, L"That file is not a material file.") == 0 &&
                        Set_ApplyMaterialDocument(settings, L"{\"format\":\"other\"}") != nullptr;
            Preview::PutSetting(L"materialUserPresets", oldPresets);
            Set_LoadFields(settings);
            Set_BuildPage(settings);
            Preview::Log(L"smoke: material preset buttons, stored JSON and file round trip: %ls",
                         presetOk ? L"PASS" : L"FAIL");
            ok = presetOk && ok;
        }
        auto saveIconSize = [&](int requested) {
            for (auto& field : g_setWin->fields) {
                if (wcscmp(field.key, L"gridIconSize") == 0) field.cur = std::to_wstring(requested);
            }
            Set_Save(g_setWin);
            LoadSettings();
            ComputeLayout();
            const auto cell = MakeSquareCell(0, 0, g_layout.cellWidth);
            return std::pair{g_settings.iconSize, cell.icon.right - cell.icon.left};
        };
        auto [smallSetting, smallIcon] = saveIconSize(24);
        auto [largeSetting, largeIcon] = saveIconSize(192);
        ok = smallSetting == 24 && largeSetting == 192 &&
             std::abs(smallIcon / g_layout.scale - 24) < 1 &&
             std::abs(largeIcon / g_layout.scale - 192) < 1 && ok;
        Preview::Log(L"smoke: saved icon size %d to %d, rendered icon %.1f to %.1f px",
                     smallSetting, largeSetting, smallIcon, largeIcon);
        Preview::PutSetting(L"gridIconSize", L"56");
        Preview::PutSetting(L"interfaceLanguage", L"zh-CN");
        LoadSettings(); g_settings.groups=false;
        ok = launchpad_i18n::chinese.load() && ok;
        g_settings.opacity = 0.96f; g_settings.backend = launchpad_material::Backend::Transparent;
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
        const auto saveButton = GroupEditButtons().back();
        const int saveX = (int)(saveButton.right - 18 * g_layout.scale);
        const int saveY = (int)((saveButton.top + saveButton.bottom) / 2);
        const auto idleSave = GetPixel(g_surface.dc, saveX, saveY);
        SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(saveX, saveY));
        Render();
        ok = GroupEditHover() == 2 && GetPixel(g_surface.dc, saveX, saveY) != idleSave &&
             SaveGrid(output + L"\\group-edit-save-hover-zh-CN.png") && ok;
        SendMessageW(hwnd, WM_MOUSELEAVE, 0, 0);
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
        auto overflow = std::find_if(g_groupGeometry.begin(), g_groupGeometry.end(), [](const auto& group) {
            return g_groupPositions.contains(group.group) &&
                GroupPageCount((int)group.group->apps.size(), group.columns, group.visibleRows) > 1;
        });
        if (overflow != g_groupGeometry.end()) {
            Render();
            ok = SaveGrid(output + L"\\group-page-indicator-zh-CN.png") && ok;
        }

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
            // The glow follows the nearest icon now, so the pointer is kept next to
            // that icon. The glass is drawn behind the icon, so only the padding
            // around it is visible: these points sit 10 DIP outside the icon edge,
            // well inside the 24 DIP reach.
            auto icon=CellAt(selectable[0]).icon;
            const LONG gap=(LONG)std::lround(10*g_layout.scale);
            const LONG cx=(LONG)std::lround((icon.left+icon.right)/2);
            const LONG cy=(LONG)std::lround((icon.top+icon.bottom)/2);
            const LONG lx=(LONG)std::lround(icon.left-gap),rx=(LONG)std::lround(icon.right+gap);
            const LONG ty=(LONG)std::lround(icon.top-gap),by=(LONG)std::lround(icon.bottom+gap);
            auto left=POINT{lx,cy},right=POINT{rx,cy};
            auto top=POINT{cx,ty},bottom=POINT{cx,by};
            auto move=[&](POINT p) {SendMessageW(hwnd,WM_MOUSEMOVE,0,MAKELPARAM(p.x,p.y));Render();};
            std::array<POINT,4> catches{left,right,top,bottom};
            for(size_t edge=0;edge<catches.size();++edge) {
                move(catches[edge]);
                bool e=g_glassClaimed&&pixel(catches[edge])>pixel(catches[edge^1])+22&&pixel(next)==neighbour;
                Preview::Log(L"smoke stage edge %d ok=%d delta=%d",(int)edge,(int)e,
                             pixel(catches[edge])-pixel(catches[edge^1]));
                ok=e&&ok;
            }
            POINT corners[4]={{lx,ty},{rx,ty},{lx,by},{rx,by}};
            for(auto corner:corners) {
                move(corner);
                // Diagonal pointers are ~14 DIP out, still inside the 24 DIP reach.
                // Brightness there is covered by the edge samples above; the corner
                // of the cell is outside the rounded glass shape, so only the claim
                // is asserted here and the screenshot is taken at a diagonal point.
                ok=g_glassClaimed&&ok;
            }
            {
                // The setting has to widen the glow: the same pointer, 60 DIP left of
                // the first icon and 26 DIP outside its cell, must leave the tile dark
                // at 0 and light it at 80. The light is drawn inside the cell, so the
                // sample sits just inside its left edge, where the clamped light lands.
                const LONG farX=(LONG)std::lround(icon.left-60*g_layout.scale);
                const LONG edgeX=(LONG)std::lround(unit.left+2*g_layout.scale);
                const POINT farPoint{farX,cy},cellEdge{edgeX,cy};
                const int savedReach=g_settings.glowDistance;
                g_settings.glowDistance=0;move(farPoint);Render();int dark=pixel(cellEdge);
                g_settings.glowDistance=80;move(farPoint);Render();int lit=pixel(cellEdge);
                g_settings.glowDistance=savedReach;
                Preview::Log(L"smoke glow distance 0=%d 80=%d",dark,lit);
                ok=lit>dark+20&&ok;
                move(left);
            }
            {
                // A small move while still over the icon must visibly light the
                // matching rim as the distance setting is increased.
                const int savedReach = g_settings.glowDistance;
                const int step = (int)std::lround(4 * g_layout.scale);
                const POINT nearCentre[] = {{cx-step,cy}, {cx+step,cy}, {cx,cy-step}, {cx,cy+step}};
                // Sample both strokes of the bevel. Their rasterised pixel
                // centres differ between the top/left and bottom/right.
                const auto sampleRim = [&](int edge) {
                    std::array<int, 16> samples{};
                    for (int i = 0; i < (int)samples.size(); ++i) {
                        const LONG d = (LONG)std::lround((i + 1) * g_layout.scale);
                        const POINT rim[] = {{(LONG)std::lround(unit.left)+d,cy},
                                             {(LONG)std::lround(unit.right)-d,cy},
                                             {cx,(LONG)std::lround(unit.top)+d},
                                             {cx,(LONG)std::lround(unit.bottom)-d}};
                        samples[i] = pixel(rim[edge]);
                    }
                    return samples;
                };
                const auto brightestIncrease = [](const auto& before, const auto& after) {
                    int increase = 0;
                    for (size_t i = 0; i < before.size(); ++i)
                        increase = std::max(increase, after[i] - before[i]);
                    return increase;
                };
                bool responsive = true;
                for (int edge = 0; edge < (int)std::size(nearCentre); ++edge) {
                    g_settings.glowDistance = 0;
                    move(nearCentre[edge]);
                    const auto low = sampleRim(edge), lowOpposite = sampleRim(edge ^ 1);
                    g_settings.glowDistance = 80;
                    move(nearCentre[edge]);
                    const auto high = sampleRim(edge);
                    const int facing = brightestIncrease(low, high);
                    const int opposite = brightestIncrease(lowOpposite, sampleRim(edge ^ 1));
                    // An outline by itself passes the peak probe but gives no
                    // light this far into the face. Verify the soft band too.
                    int band = 255;
                    for (int i = 6; i <= 9; ++i)
                        band = std::min(band, high[i] - low[i]);
                    responsive = responsive && g_glassClaimed && facing > 20 &&
                                 facing > opposite + 20 && band > 12 && pixel(next) == neighbour;
                    Preview::Log(L"smoke: near-centre rim %d, peak=%d band at 7–10 DIP=%d opposite=%d",
                                 edge, facing, band, opposite);
                    if (edge == 1)
                        ok = SaveGrid(output + L"\\square-grid-glow-near-centre.png") && ok;
                }
                const LONG rimX = (LONG)std::lround(unit.left + 2 * g_layout.scale);
                move({rimX, cy});
                ok = SaveGrid(output + L"\\square-grid-glow-face-edge.png") && ok;
                g_settings.glowDistance = savedReach;
                ok = responsive && ok;
                Preview::Log(L"smoke: app grid glow reaches into all four facing edges: %ls",
                             responsive ? L"PASS" : L"FAIL");
                move(left);
            }
            Preview::Log(L"smoke stage corners ok=%d",(int)ok);
            ok=SaveGrid(output+L"\\glass-corner-zh-CN.png")&&ok;
            move(left);ok=SaveGrid(output+L"\\square-grid-hover-zh-CN.png")&&ok;

            double started=Now();
            for(int frame=0;frame<24;++frame) {
                auto slot=CellAt(selectable[frame%2]).icon;
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
                move(sample(cell.icon,.5f,.5f));
                const float wantX=(cell.icon.left+cell.icon.right)/2;
                const float wantY=(cell.icon.top+cell.icon.bottom)/2;
                const float wantU=(wantX-cell.unit.left)/std::max(1.f,cell.unit.right-cell.unit.left);
                const float wantV=(wantY-cell.unit.top)/std::max(1.f,cell.unit.bottom-cell.unit.top);
                auto fade=std::find_if(std::begin(g_glowFades),std::end(g_glowFades),[&](const auto& slot) {
                    return slot.key==g_visible[selectable[0]].app.get()&&slot.alpha>0;
                });
                ok=g_glassClaimed&&fade!=std::end(g_glowFades)&&std::abs(fade->light.x-wantU)<.02f&&
                    std::abs(fade->light.y-wantV)<.02f&&InRect(cell.unit,cell.icon.left,cell.icon.top)&&ok;
            }
            SendMessageW(hwnd,WM_MOUSELEAVE,0,0);
            g_settings.iconSize=iconSize;g_settings.labelSize=labelSize;
            g_layout.scale=1;g_scroll=g_scrollTarget=0;
            EnsureFormats(1);ComputeLayout();
            g_settings.tint=originalTint;g_settings.opacity=originalOpacity;
            SendMessageW(hwnd,WM_MOUSELEAVE,0,0);
            Render();
            auto one=CellAt(selectable[0]).highlight,two=CellAt(selectable[1]).highlight;
            g_selectionPointer.pending=true;
            g_selectionPointer.start={(LONG)std::min(one.left,two.left)-5,(LONG)std::min(one.top,two.top)-5};
            MoveAppSelection((int)std::max(one.right,two.right)+5,(int)std::max(one.bottom,two.bottom)+5);
            ok=AppSelected(*g_visible[selectable[0]].app)&&AppSelected(*g_visible[selectable[1]].app)&&ok;
            Render();ok=SaveGrid(output+L"\\batch-selection-zh-CN.png")&&ok;
            ClearAppSelection();
        } else ok=false;
        g_groupEditMode=false;g_settings.backend=launchpad_material::Backend::Transparent;g_settings.opacity=1;g_settings.tint=D2D1::ColorF(0xFFFFFF);
        Render();ok=SaveGrid(output+L"\\readability-white-background.png")&&ok;
        g_settings.tint=D2D1::ColorF(0x111318);Render();ok=SaveGrid(output+L"\\readability-dark-background.png")&&ok;
        g_settings.tint=D2D1::ColorF(0x808080);Render();ok=SaveGrid(output+L"\\readability-gray-background.png")&&ok;
        if (!selectable.empty()) {
            const int index = selectable.front();
            const auto& tile = g_visible[index];
            const auto key = MotionKey(tile);
            const auto savedEffects = g_settings.textEffects;
            g_settings.textEffects.shadowOpacity = 72;
            g_settings.textEffects.outlineOpacity = 0;

            // Cold open: measure the same app label before the graphics device
            // exists, as ComputeLayout does before the first Render in Open.
            ClearTileLabelCache();
            ComPtr<ID2D1Device> device;
            g_d2dDevice.Swap(device);
            ComputeLayout();
            const auto cell = CellAt(index);
            auto& cold = TileLabel(tile, cell.label, cell.wrapLabel);
            const auto originalLayout = cold.layout;
            const bool measuredBeforeGraphics = !g_d2dDevice && originalLayout && !cold.shadow.bitmap &&
                                                cold.shadow.attempts == 0;
            g_d2dDevice.Swap(device);
            Render();
            const auto coldEntry = g_labelCache.find(key);
            const bool firstFrame = coldEntry != g_labelCache.end() &&
                                    coldEntry->second.layout.Get() == originalLayout.Get() &&
                                    coldEntry->second.shadow.bitmap;

            // A temporarily unavailable shadow device must queue a frame
            // even with launcher animations disabled and no mouse input.
            ClearTileLabelCache();
            ClearLauncherTextCache();
            g_d2dDevice.Swap(device);
            Render();
            const auto missing = g_labelCache.find(key);
            const auto layoutAfterFailure = missing == g_labelCache.end()
                ? ComPtr<IDWriteTextLayout>{} : missing->second.layout;
            const bool retryQueued = !g_d2dDevice && layoutAfterFailure && !missing->second.shadow.bitmap &&
                                     missing->second.shadow.attempts == 1 && g_textShadowRetryPending &&
                                     g_frameTimer && !Animate();
            g_d2dDevice.Swap(device);
            OnFrame();
            const auto recovered = g_labelCache.find(key);
            const bool recoveredInPlace = recovered != g_labelCache.end() &&
                recovered->second.layout.Get() == layoutAfterFailure.Get() &&
                recovered->second.shadow.bitmap && recovered->second.shadow.attempts == 2 &&
                !g_textShadowRetryPending;

            const bool shadowOk = measuredBeforeGraphics && firstFrame && retryQueued && recoveredInPlace;
            Preview::Log(L"smoke: cold label shadow and same-open retry: %ls (layout=%d first=%d queued=%d recovered=%d)",
                         shadowOk ? L"PASS" : L"FAIL", measuredBeforeGraphics, firstFrame,
                         retryQueued, recoveredInPlace);
            ok = shadowOk && ok;
            g_settings.textEffects = savedEffects;
            ClearTileLabelCache();
            ClearLauncherTextCache();
        } else ok = false;
        // Exercise the common white labels over a synthetic split-luminance background.
        std::vector<UINT32> mixed(160*100);
        for(int y=0;y<100;y++)for(int x=0;x<160;x++)mixed[y*160+x]=x<80?0xFFF5F5F5:0xFF090909;
        ComPtr<ID2D1Bitmap> mixedBitmap;
        auto mixedProps=D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),96,96);
        ok=SUCCEEDED(g_target->CreateBitmap(D2D1::SizeU(160,100),mixed.data(),160*4,mixedProps,&mixedBitmap))&&ok;
        mixedBitmap.As(&g_materialCache.blurred);g_materialCache.width=160;g_materialCache.height=100;g_materialCache.sourceWidth=160;g_materialCache.sourceHeight=100;
        g_material.average = AverageBackdropColor(mixed.data(), 160, 100);
        g_settings.backend=launchpad_material::Backend::Custom;g_settings.opacity=0;g_settings.backgroundDim=0;

        const auto previousTextEffects = g_settings.textEffects;
        auto checkTextEffects = [&](TextEffects effects, const wchar_t* filename) {
            g_settings.textEffects = effects;
            g_labelCache.clear();
            ClearLauncherTextCache();
            Render();
            return SaveGrid(output + filename);
        };
        auto shadowOnly = previousTextEffects;
        shadowOnly.outlineOpacity = 0;
        auto outlineOnly = previousTextEffects;
        outlineOnly.shadowOpacity = 0;
        ok = checkTextEffects(shadowOnly, L"\\readability-shadow-only.png") && ok;
        ok = checkTextEffects(outlineOnly, L"\\readability-outline-only.png") && ok;
        ok = checkTextEffects(previousTextEffects, L"\\readability-mixed-background.png") && ok;
        for(float progress:{0.0f,.25f,.5f,.75f,1.0f}){g_appear=progress;Render();}
        g_closing=true;for(float progress:{.75f,.5f,.25f}){g_appear=progress;Render();}g_closing=false;g_appear=1;

        // Menus opened on different halves of the page share its overall color.
        g_settings.backend=launchpad_material::Backend::Custom;g_settings.opacity=0.35f;g_settings.tint=D2D1::ColorF(0x11111B);
        Render();
        ok=PreviewMenuShot(output+L"\\menu-tile-zh-CN.png",g_layout.area.left+180,g_layout.area.top+140)&&ok;
        auto menuColor = LaunchpadMenu::g_menu.background;
        Render();
        ok=PreviewMenuShot(output+L"\\menu-hover-zh-CN.png",g_layout.area.left+920,g_layout.area.top+140,2)&&ok;
        const auto otherColor = LaunchpadMenu::g_menu.background;
        ok = menuColor.r == otherColor.r && menuColor.g == otherColor.g && menuColor.b == otherColor.b && ok;
        Render();
        ok=PreviewMenuShot(output+L"\\menu-submenu-zh-CN.png",g_layout.area.left+180,g_layout.area.top+140,1,1)&&ok;
        if (g_layout.powerItems.size() >= 2) {
            // The smoke image uses the first six actions; enable every warning
            // action so the first button is representative too. Restored below.
            const auto warningActions = g_settings.powerWarningActions;
            for (auto& action : g_settings.powerWarningActions) action = true;
            const auto first = PowerButtonRect(0), second = PowerButtonRect(1);
            const int centerX = (int)((first.left + first.right) / 2);
            const int centerY = (int)((first.top + first.bottom) / 2);
            const int x = centerX;
            const int y = centerY;
            const int neighborX = (int)((second.left + second.right) / 2);
            const int outsideX = (int)(g_layout.powerBar.left - 4 * g_layout.scale);
            const int outsideY = (int)((g_layout.powerBar.top + g_layout.powerBar.bottom) / 2);
            const int farY = (int)(g_layout.powerBar.top -
                (g_settings.glowDistance + 16) * g_layout.scale);
            const int dockX = (int)((g_layout.powerBar.left + g_layout.powerBar.right) / 2);
            SendMessageW(hwnd, WM_MOUSELEAVE, 0, 0);
            Render();
            const int probeY = centerY + (int)std::lround(15 * g_layout.scale);
            const auto idleCenter = GetPixel(g_surface.dc, centerX, probeY);
            const auto idleOutside = GetPixel(g_surface.dc, outsideX, outsideY);
            SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
            SettlePowerGlass();
            Render();
            const auto litCenter = GetPixel(g_surface.dc, centerX, probeY);
            const auto neighbor = GetPixel(g_surface.dc, neighborX, probeY);
            const auto outside = GetPixel(g_surface.dc, outsideX, outsideY);
            auto redness = [](COLORREF c) {
                return 2 * (int)GetRValue(c) - (int)GetGValue(c) - (int)GetBValue(c);
            };
            // One capsule-wide lobe: the hovered button is the hottest point,
            // the neighbour only catches the falloff, and nothing leaves the
            // glass pill.
            const bool hoverIsLocal = g_powerHover == 0 &&
                litCenter != idleCenter &&
                redness(litCenter) > redness(neighbor) &&
                outside == idleOutside;
            Preview::Log(L"smoke: power hover local details hover=%d changed=%d red=%d/%d outside=%d",
                         g_powerHover, litCenter != idleCenter, redness(litCenter), redness(neighbor),
                         outside == idleOutside);
            ok = hoverIsLocal && SaveGrid(output + L"\\power-hover-zh-CN.png") && ok;
            // The warning glass is one shared layer: it moves to the next
            // button instead of two separate backgrounds.
            SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(neighborX, y));
            SettlePowerGlass();
            Render();
            ok = g_powerHover == 1 && SaveGrid(output + L"\\power-hover-restart-zh-CN.png") && ok;
            if (g_layout.powerItems.size() >= 3) {
                const auto last = PowerButtonRect((int)g_layout.powerItems.size() - 1);
                const int lastX = (int)((last.left + last.right) / 2);
                SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(lastX, y));
                SettlePowerGlass();
                Render();
                ok = g_powerHover == (int)g_layout.powerItems.size() - 1 &&
                     SaveGrid(output + L"\\power-hover-right-zh-CN.png") && ok;
            }
            // Kept as visual variants of the same hover state; the warning
            // tint now lives inside the capsule, so the backdrop no longer
            // drives a second material pass.
            for (int theme = 0; theme < 3; ++theme) {
                SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
                SettlePowerGlass();
                Render();
                ok = SaveGrid(output + (theme == 0 ? L"\\power-hover-dark-zh-CN.png"
                                         : theme == 1 ? L"\\power-hover-light-zh-CN.png"
                                                      : L"\\power-hover-color-zh-CN.png")) && ok;
            }
            SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
            SettlePowerGlass();
            Render();
            const auto hovered = GetPixel(g_surface.dc, centerX + (int)(13 * g_layout.scale),
                                          centerY);
            g_settings.powerNoConfirm = false;
            const auto oldOrder = g_settings.powerOrder;
            const int firstAction = g_layout.powerItems[0], secondAction = g_layout.powerItems[1];
            SendMessageW(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
            SettlePowerGlass();
            Render();
            // Press compresses the lobe without brightening it, so the same
            // off-glyph probe must not get redder.
            ok = g_powerPressed == firstAction && g_powerGlass.press > .9f &&
                 redness(GetPixel(g_surface.dc, centerX + (int)(13 * g_layout.scale), centerY)) <=
                     redness(hovered) &&
                 SaveGrid(output + L"\\power-press-zh-CN.png") && ok;
            std::iter_swap(std::find(g_settings.powerOrder.begin(), g_settings.powerOrder.end(), firstAction),
                           std::find(g_settings.powerOrder.begin(), g_settings.powerOrder.end(), secondAction));
            ComputeLayout();
            SendMessageW(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
            ok = g_powerArmed == -1 && ok;
            g_settings.powerOrder = oldOrder;
            ComputeLayout();
            SendMessageW(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
            SendMessageW(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
            SettlePowerGlass();
            Render();
            ok = g_powerArmed == firstAction && SaveGrid(output + L"\\power-confirm-zh-CN.png") && ok;
            SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(dockX, farY));
            ok = g_powerHover == -1 && PowerAt(dockX, farY) == -1 && ok;
            DisarmPower();
            SendMessageW(hwnd, WM_MOUSELEAVE, 0, 0);
            ResetPowerGlass();
            Render();
            for (int i = 0; i < (int)std::size(g_settings.powerWarningActions); ++i)
                g_settings.powerWarningActions[i] = warningActions[i];
            Preview::Log(L"smoke: only the hovered power button lit up: %ls", hoverIsLocal ? L"PASS" : L"FAIL");
        }
        // Verify the three-button arrangement with the production renderer.
        {
            const auto saved = g_settings;
            g_settings.backend = launchpad_material::Backend::Transparent;
            g_settings.opacity = 1.f;
            g_settings.tint = D2D1::ColorF(0x25252D);
            for (int action = 0; action < (int)std::size(kPower); ++action) {
                g_settings.powerShown[action] =
                    action == (int)PowerAction::SignOut ||
                    action == (int)PowerAction::Restart ||
                    action == (int)PowerAction::ShutDown;
                g_settings.powerWarningActions[action] = true;
            }
            ComputeLayout();
            bool centred = g_layout.powerItems.size() == 3;
            const float s = g_layout.scale;
            const int tipY = (int)std::round(g_layout.powerBar.top - std::round(8.f * s) - 4.f * s);
            const int dx = (int)std::round(10.f * s);
            for (int slot = 0; slot < (int)g_layout.powerItems.size(); ++slot) {
                const auto button = PowerButtonRect(slot);
                const int x = (int)std::round((button.left + button.right) * .5f);
                const int y = (int)std::round((button.top + button.bottom) * .5f);
                SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(x + dx, y));
                SettlePowerGlass();
                const auto sample = [&](int offset) {
                    const auto pixel = GetPixel(g_surface.dc, x + offset, tipY);
                    return 2 * (int)GetRValue(pixel) - (int)GetGValue(pixel) - (int)GetBValue(pixel);
                };
                g_settings.powerWarningBleed = 0;
                Render();
                const int off[] = {sample(-dx), sample(0), sample(dx - 1)};
                g_settings.powerWarningBleed = 100;
                Render();
                const int left = sample(-dx) - off[0];
                const int centre = sample(0) - off[1];
                const int right = sample(dx - 1) - off[2];
                Preview::Log(L"smoke: warning reflection slot %d, left=%d centre=%d right=%d",
                             slot, left, centre, right);
                centred = g_powerHover == slot && centre > left && centre > right &&
                          std::abs(left - right) <= std::max(5, centre / 20) && centred;
                g_settings.powerWarningBleed = saved.powerWarningBleed;
                Render();
                ok = SaveGrid(output + L"\\power-three-" + std::to_wstring(slot) + L".png") && ok;
            }
            if (g_layout.powerItems.size() == 3) {
                const auto start = PowerButtonRect(0), next = PowerButtonRect(1);
                const int x0 = (int)std::round((start.left + start.right) * .5f);
                const int x1 = (int)std::round((next.left + next.right) * .5f);
                const int y = (int)std::round((start.top + start.bottom) * .5f);
                const bool requested = g_requestedAnimations.exchange(true);
                const bool system = g_systemAnimations.exchange(true);
                const int gapX = (x0 + x1) / 2;
                auto move = [&](int x) { SendMessageW(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y)); };
                const auto red = [&](int x, int dy) {
                    const COLORREF p = GetPixel(g_surface.dc, x, y + dy);
                    return 2 * (int)GetRValue(p) - (int)GetGValue(p) - (int)GetBValue(p);
                };
                // Sample the lobes below the opaque parts of their glyphs,
                // on the same row as the dark shoulder of the neck.
                const int sourceX = x0 + 5, arrivalX = x1 - 5, lobeY = 11;
                SendMessageW(hwnd, WM_MOUSELEAVE, 0, 0);
                SettlePowerGlass();
                Render();
                const int sourceBase = red(sourceX, lobeY), arrivalBase = red(arrivalX, lobeY);
                const int neckBase = red(gapX, 0), offNeckBase = red(gapX, 11);
                move(x0);
                SettlePowerGlass();
                Render();
                g_settings.powerWarningFollowButton = true;
                move(x0);
                SettlePowerGlass();
                Render();
                const auto renderAt = [&](int x, double elapsed) {
                    move(x);
                    g_powerGlass.tick = Now() - elapsed;
                    Render();
                };
                // These three samples are geometrically outside either hit
                // circle, even though the physical pointer moves each time.
                bool gapTracks = true;
                float shares[3]{};
                int necks[3]{};
                for (int offset = -1; offset <= 1; ++offset) {
                    renderAt(gapX + offset * 2, .08);
                    shares[offset + 1] = g_powerGlass.weights[1];
                    necks[offset + 1] = red(gapX, 0) - neckBase;
                    gapTracks = gapTracks && g_powerHover == -1 && PowerAt(gapX + offset * 2, y) == -1 &&
                                g_powerGlass.litSlot == 0 && g_powerGlass.targetX == (float)x0;
                    if (offset == 0) {
                        const int tail = red(sourceX, lobeY) - sourceBase;
                        const int head = red(arrivalX, lobeY) - arrivalBase;
                        const int shoulder = red(gapX, 11) - offNeckBase;
                        gapTracks = gapTracks && tail > 20 && head > 20 &&
                                    necks[1] > shoulder + 15 &&
                                    tail > shoulder + 12 && head > shoulder + 12;
                    }
                }
                renderAt(gapX - 2, .08);
                const bool reverses = g_powerHover == -1 && g_powerGlass.weights[1] < shares[1];
                gapTracks = gapTracks && shares[1] > shares[0] + .10f &&
                            shares[2] > shares[1] + .10f && necks[1] > 15;
                renderAt(x1, .25);
                renderAt(x1, .25);
                const bool collapses = g_powerGlass.weights[0] == 0.f &&
                                       g_powerGlass.weights[1] == 1.f &&
                                       g_powerGlass.x == (float)x1;
                Preview::Log(L"smoke: button-mode transfer in the non-clickable gap: %ls (%.2f %.2f %.2f)",
                             gapTracks && reverses && collapses ? L"PASS" : L"FAIL",
                             shares[0], shares[1], shares[2]);
                ok = gapTracks && reverses && collapses && ok;

                // Capture the physical path in both directions rather than
                // generating frames from a fixed hover destination.
                move(x0);
                SettlePowerGlass();
                const int path[] = {x0, x0+12, x0+15, gapX-4, gapX-2, gapX,
                                    gapX+2, gapX+4, x1-14, x1-11, x1, x1,
                                    x1-12, gapX+4, gapX+2, gapX, gapX-2,
                                    gapX-4, x0+14, x0+11, x0, x0};
                for (int frame = 0; frame < (int)std::size(path); ++frame) {
                    renderAt(path[frame], .055);
                    ok = SaveGrid(output + L"\\power-pointer-transfer-" + std::to_wstring(frame) + L".png") && ok;
                }
                g_requestedAnimations = requested;
                g_systemAnimations = system;
            }
            SendMessageW(hwnd, WM_MOUSELEAVE, 0, 0);
            ResetPowerGlass();
            g_settings = saved;
            ComputeLayout();
            Render();
            ok = centred && ok;
            Preview::Log(L"smoke: three warning reflections centred above their buttons: %ls",
                         centred ? L"PASS" : L"FAIL");
        }
        auto updateDots = [&] {
            unsigned count = 0;
            for (int slot = 0; slot < (int)g_layout.powerItems.size(); ++slot) {
                auto button = PowerButtonRect(slot);
                int x = (int)std::round(button.right - 10 * g_layout.scale);
                int y = (int)std::round(button.top + 10 * g_layout.scale);
                COLORREF pixel = GetPixel(g_surface.dc, x, y);
                bool yellow = GetRValue(pixel) > 200 && GetGValue(pixel) > 100 && GetBValue(pixel) < 120;
                if (yellow) {
                    ++count;
                    ok = IsUpdatePowerAction(static_cast<PowerAction>(g_layout.powerItems[slot])) && ok;
                }
            }
            return count;
        };
        SendMessageW(hwnd, kPowerUpdateStatusReady, static_cast<WPARAM>(PowerUpdateStatus::RestartRequired), 0);
        ok = updateDots() == 2 && SaveGrid(output + L"\\power-update-zh-CN.png") && ok;
        SendMessageW(hwnd, kPowerUpdateStatusReady, static_cast<WPARAM>(PowerUpdateStatus::Unknown), 0);
        ok = updateDots() == 2 && ok;
        SendMessageW(hwnd, kPowerUpdateStatusReady, static_cast<WPARAM>(PowerUpdateStatus::NoRestartPending), 0);
        ok = updateDots() == 0 && ok;
        g_materialCache={};
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
    bool liveSettings = false;
    int count = 0;
    auto argv = CommandLineToArgvW(GetCommandLineW(), &count);
    for (int i = 1; i < count; i++) {
        std::wstring arg = argv[i];
        if (arg == L"--settings") Preview::settingsOnStart = true;
        if (arg == L"--smoke") Preview::smoke = true;
        if (arg == L"--live-settings") liveSettings = true;
        if (arg == L"--lang" && i+1 < count) Preview::languageOverride = argv[++i];
    }
    LocalFree(argv);
    if (liveSettings) {
        // A real window, not an off-screen surface: the window region, the frame and the
        // acrylic only exist on a live HWND, so this is the only way to look at the corner.
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (!Set_Create(false)) {
            Preview::Log(L"live settings: Set_Create(false) failed, GetLastError=%lu", GetLastError());
            return 3;
        }
        Preview::Log(L"live settings: window %p", (void*)g_setWin->hwnd);
        Set_LoadFields(*g_setWin);
        HWND h = g_setWin->hwnd;
        SetWindowPos(h, HWND_TOPMOST, 80, 80, 1000, 760, SWP_SHOWWINDOW | SWP_NOACTIVATE);
        DWORD until = GetTickCount() + 12000;
        MSG message;
        while (GetTickCount() < until) {
            while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }
            Sleep(10);
        }
        DestroyWindow(h);
        return 0;
    }
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

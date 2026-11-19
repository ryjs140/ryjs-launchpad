#define SEAMLESS_PREVIEW 1
#include "../mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp"
#include <cstdio>
void Preview_RunSmoke(HWND) {}
int failures = 0, checks = 0;
void Check(bool condition, const char* label) {
    ++checks;
    if (!condition) { ++failures; printf("FAIL: %s\n", label); }
}
std::string Bytes(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(file), {});
}
int wmain(int argc, wchar_t** argv) {
    if (argc != 3) return 2;
    namespace fs = std::filesystem;
    fs::path source = fs::absolute(argv[1]);
    fs::path cases = fs::absolute(argv[2]) / std::to_wstring(GetTickCount64());
    fs::create_directories(cases);
    auto copied = cases / L"data-v1";
    Check(ryjs_data::Prepare(source.wstring(), copied.wstring(), [](const auto&) {return true;}), "complete transactional copy");
    Check(ryjs_data::Prepare(L"", copied.wstring(), [](const auto&) {return false;}), "restart uses existing valid data without reimport");
    for (const auto& entry:fs::directory_iterator(source)) {
        if (entry.is_regular_file() && entry.path().extension() != L".tmp" && entry.path().filename()!=L"ryjs-storage.ini" && !entry.path().filename().wstring().starts_with(L"performance"))
            Check(Bytes(entry.path()) == Bytes(copied/entry.path().filename()), "migrated file byte-for-byte preservation");
    }
    Preview::directory = copied.wstring();
    for (const auto& entry:fs::directory_iterator(copied)) {
        auto name = entry.path().filename().wstring();
        if (entry.path().extension()!=L".txt" || (!name.starts_with(L"groups-v1-") && !name.starts_with(L"layout-"))) continue;
        g_settings.groups = name.starts_with(L"groups-v1-");
        int sourceIndex = name.find(L"pinned")!=std::wstring::npos ? 3 : name.find(L"desktop")!=std::wstring::npos ? 1 : name.find(L"both")!=std::wstring::npos ? 2 : 0;
        g_appsKey = sourceIndex << 16;
        g_apps.clear(); g_tiles.clear(); g_pins.clear(); g_hidden.clear();
        g_dataReadOnly = g_layoutReadOnly = false;
        auto original = Bytes(entry.path());
        auto records = ReadLines(entry.path().wstring());
        std::vector<std::wstring> expected;
        std::set<std::wstring> targets;
        for (const auto& row:records) {
            std::wstring line;
            for (size_t i=0;i<row.size();++i) line+=(i?L"\t":L"")+row[i];
            expected.push_back(line);
            if (row.size()>1 && (row[0]==L"app" || row[0]==L"in") && targets.insert(ToLower(row[1])).second) {
                auto app=std::make_shared<App>();app->target=row[1];app->name=row[1];g_apps.push_back(app);
            }
        }
        LoadTiles();
        Check(!g_dataReadOnly && expected==ArrangementLines(), "layout parse preserves groups, names, membership and order");
        size_t folders = std::count_if(g_tiles.begin(),g_tiles.end(),[](const auto&t){return t.folder!=nullptr;});
        printf("layout: %ls, groups=%zu, applications=%zu\n",name.c_str(),folders,targets.size());
        Check(SaveTiles() && Bytes(entry.path())==original, "saving unchanged layout keeps exact file bytes");
        g_tiles.clear();LoadTiles();
        Check(expected==ArrangementLines(), "restart/readback preserves layout");
        if (g_settings.groups && !g_apps.empty()) {
            auto absent=g_apps.front();g_apps.erase(g_apps.begin());LoadTiles();
            Check(expected==ArrangementLines() && SaveTiles(), "missing application keeps group and ordinal");
            g_apps.insert(g_apps.begin(),absent);LoadTiles();
            Check(expected==ArrangementLines(), "returning application rejoins original position");
        }
        auto valid=ArrangementLines();
        {std::ofstream file(entry.path(),std::ios::binary);file.write("\xff\xfe\xff",3);}
        LoadTiles();
        Check(g_dataReadOnly && g_layoutReadOnly && !SaveTiles() && ArrangementLines()==valid && Bytes(entry.path())==std::string("\xff\xfe\xff",3),
              "unreadable UTF-8 preserves memory and blocks overwrite");
        {std::ofstream file(entry.path(),std::ios::binary);file.write(original.data(),original.size());}
        g_dataReadOnly=g_layoutReadOnly=false;
    }
    auto failed=cases/L"failed-copy";
    Check(!ryjs_data::Prepare(source.wstring(),failed.wstring(),[](const auto&){return false;}) && !fs::exists(failed) && fs::exists(failed.wstring()+L".pending"),
          "failed conversion never commits partial data");
    Check(ryjs_data::Prepare(source.wstring(),failed.wstring(),[](const auto&){return true;}) &&
          fs::exists(failed) && !fs::exists(failed.wstring()+L".pending"),
          "interrupted migration discards the partial copy and retries");
    auto marker=(copied/L"ryjs-storage.ini").wstring();
    WritePrivateProfileStringW(L"storage",L"schema",L"999",marker.c_str());
    Check(!ryjs_data::Prepare(source.wstring(),copied.wstring(),[](const auto&){return true;}), "future storage schema never replaced");
    Check(!ryjs_data::Validate(L"groups-v1-allApps.txt",L"schema\t999\tgroups\nfuture\tkeep"), "future group schema rejected");
    Check(!ryjs_data::Validate(L"groups-v1-allApps.txt",L"schema\t1\tgroups\nin\torphan"), "orphan application record rejected");
    Check(ryjs_data::Validate(L"layout-allApps.txt",L""), "an empty legacy layout is a valid empty list");
    Check(!ryjs_data::Validate(L"groups-v1-allApps.txt",L""), "an empty group layout is rejected");
    auto privateSettings=(cases/L"private.ini").wstring();
    Check(launchpad_settings_store::Save(privateSettings,{{L"uiFont",L"中文字体",L"auto"},{L"program",L"\"C:\\中文\\app.exe\" --arg",L""}}), "Unicode and quoted settings save");
    auto before=Bytes(privateSettings);
    Check(launchpad_settings_store::ResolveReadOnly(privateSettings,L"uiFont",L"other")==L"other" && Bytes(privateSettings)==before,
          "reading inherited settings never invalidates old override file");
    Check(launchpad_settings_store::ResolveReadOnly(privateSettings,L"uiFont",L"auto")==L"中文字体", "effective inherited override retained");
    printf("%s: %d checks, %d failures\n",failures?"FAIL":"PASS",checks,failures);
    return failures?1:0;
}

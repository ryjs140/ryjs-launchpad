#include <windows.h>
#include <cstdio>
#include <string>
#include "../mods/local/ryjs-launchpad/ryjs_data.h"

int wmain(int argc, wchar_t** argv) {
    if (argc < 2) return 2;
    std::wstring directory = argv[1];
    std::wstring marker = directory + L"\\ryjs-storage.ini";
    int schema = GetPrivateProfileIntW(L"storage", L"schema", 0, marker.c_str());
    bool valid = ryjs_data::ValidateDirectory(directory);
    printf("schema=%d validDirectory=%d\n", schema, valid ? 1 : 0);
    // Report each file that would be rejected, so the reason is visible.
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(directory)) {
        auto name = entry.path().filename().wstring();
        if (name.starts_with(L"groups-") && !name.starts_with(L"groups-v1-")) {
            wprintf(L"REJECT versioned-group name: %ls\n", name.c_str());
            continue;
        }
        if (!entry.is_regular_file() || entry.path().extension() != L".txt") continue;
        std::wstring text;
        if (!ryjs_data::ReadUtf8(entry.path().wstring(), text)) {
            wprintf(L"REJECT unreadable: %ls\n", name.c_str());
        } else if (!ryjs_data::Validate(name, text)) {
            wprintf(L"REJECT invalid: %ls\n", name.c_str());
        }
    }
    return valid && schema == ryjs_data::kStorageVersion ? 0 : 1;
}

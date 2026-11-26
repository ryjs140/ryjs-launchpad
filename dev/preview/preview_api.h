#pragma once
// The preview never links the Windhawk engine. Its narrow storage bridge is
// backed by INI files under the executable, not Windhawk or Explorer settings.
#include <windows.h>
#include <shlobj.h>
#include <cstdarg>
#include <cwchar>
#include <mutex>
#include <string>

#define WH_MOD_ID L"ryjs-launchpad"
#define WH_MOD_VERSION L"1.5.9-preview"
namespace Preview {
inline std::wstring directory;
inline std::wstring languageOverride;
inline bool settingsOnStart = false;
inline bool smoke = false;
inline bool uiCheck = false;
inline bool smokePassed = false;
inline bool benchmarkGpu = false;
inline std::recursive_mutex storageMutex;
inline std::wstring File(PCWSTR name) { return directory + L"\\" + name; }
inline void Init() {
    WCHAR path[32768];
    GetModuleFileNameW(nullptr, path, ARRAYSIZE(path));
    directory = path;
    directory.resize(directory.find_last_of(L"\\/"));
    directory += L"\\preview-data";
    SHCreateDirectoryExW(nullptr, directory.c_str(), nullptr);
}
inline std::wstring Setting(PCWSTR key) {
    if (wcscmp(key, L"interfaceLanguage") == 0 && !languageOverride.empty()) return languageOverride;
    std::lock_guard lock(storageMutex);
    WCHAR value[4096];
    GetPrivateProfileStringW(L"settings", key, L"", value, ARRAYSIZE(value), File(L"settings.ini").c_str());
    return value;
}
inline bool PutSetting(PCWSTR key, const std::wstring& value) {
    std::lock_guard lock(storageMutex);
    // Unicode INI header, otherwise Win32 profile writes use the legacy ANSI codepage.
    auto path = File(L"settings.ini");
    if (GetFileAttributesW(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
        HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (h != INVALID_HANDLE_VALUE) { WORD bom = 0xFEFF; DWORD n; WriteFile(h, &bom, 2, &n, nullptr); CloseHandle(h); }
    }
    return WritePrivateProfileStringW(L"settings", key, value.c_str(), path.c_str()) != FALSE;
}
inline void Log(PCWSTR message, ...) {
    WCHAR buffer[4096];
    va_list args; va_start(args, message);
    _vsnwprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, message, args);
    va_end(args);
    OutputDebugStringW(buffer);
    std::lock_guard lock(storageMutex);
    FILE* file = _wfopen(File(L"preview.log").c_str(), L"a, ccs=UTF-8");
    if (file) { fwprintf(file, L"%ls\n", buffer); fclose(file); }
}
} // namespace Preview
void Preview_RunSmoke(HWND hwnd);
#define Wh_Log(...) Preview::Log(__VA_ARGS__)
inline size_t Wh_GetModStoragePath(PWSTR buffer, size_t count) {
    if (Preview::directory.size() + 1 > count) return 0;
    wcscpy_s(buffer, count, Preview::directory.c_str());
    return Preview::directory.size();
}
inline int Wh_GetIntValue(PCWSTR key, int fallback) {
    std::lock_guard lock(Preview::storageMutex);
    return GetPrivateProfileIntW(L"state", key, fallback, Preview::File(L"state.ini").c_str());
}
inline BOOL Wh_SetIntValue(PCWSTR key, int value) {
    std::lock_guard lock(Preview::storageMutex);
    return WritePrivateProfileStringW(L"state", key, std::to_wstring(value).c_str(), Preview::File(L"state.ini").c_str());
}
inline int Wh_GetIntSetting(PCWSTR format, ...) {
    WCHAR key[256]; va_list args; va_start(args, format);
    _vsnwprintf_s(key, ARRAYSIZE(key), _TRUNCATE, format, args); va_end(args);
    return _wtoi(Preview::Setting(key).c_str());
}

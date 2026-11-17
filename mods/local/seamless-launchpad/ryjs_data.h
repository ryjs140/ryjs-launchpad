#pragma once
#include <filesystem>
#include <functional>
#include <fstream>
#include <stdexcept>

namespace ryjs_data {
namespace fs = std::filesystem;
inline constexpr int kStorageVersion = 1;

inline bool ReadUtf8(const std::wstring& path, std::wstring& text) {
    HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                              nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    LARGE_INTEGER length;
    bool ok = GetFileSizeEx(file, &length) && length.QuadPart <= (8 << 20) && length.QuadPart >= 0;
    std::string bytes(ok ? (size_t)length.QuadPart : 0, '\0');
    DWORD read = 0;
    ok = ok && ReadFile(file, bytes.data(), (DWORD)bytes.size(), &read, nullptr) && read == bytes.size();
    CloseHandle(file);
    if (!ok) return false;
    int count = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, bytes.data(), (int)bytes.size(), nullptr, 0);
    if (!bytes.empty() && !count) return false;
    text.resize(count);
    if (count) MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, bytes.data(), (int)bytes.size(), text.data(), count);
    if (!text.empty() && text.front() == 0xFEFF) text.erase(0, 1);
    return text.find(L'\0') == std::wstring::npos;
}

inline bool Validate(const std::wstring& name, const std::wstring& text) {
    bool layout = name.starts_with(L"layout-") || name.starts_with(L"groups-v1-");
    bool groups = name.starts_with(L"groups-v1-");
    if (!layout && name != L"pins.txt" && name != L"icons.txt") return true;
    if (layout && text.empty()) return false; // Existing empty/corrupt layouts must not regenerate.
    size_t pos = 0, rows = 0;
    bool folder = false;
    while (pos < text.size()) {
        size_t end = text.find(L'\n', pos);
        if (end == std::wstring::npos) end = text.size();
        auto line = text.substr(pos, end - pos); pos = end + 1;
        if (!line.empty() && line.back() == L'\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::wstring> f;
        size_t begin = 0;
        for (;;) {
            auto tab = line.find(L'\t', begin);
            f.push_back(line.substr(begin, tab == std::wstring::npos ? tab : tab - begin));
            if (tab == std::wstring::npos) break;
            begin = tab + 1;
        }
        if (f.size() < 2 || f[1].empty()) return false;
        if (f[0] == L"schema") {
            if (!groups || rows || f.size() != 3 || f[1] != L"1" || f[2] != L"groups") return false;
        } else if (groups && !rows) return false;
        else if (layout && f[0] == L"folder") {
            if (f.size() != 2 && !(f.size() == 3 && f[2] == L"inbox")) return false;
            folder = true;
        } else if (layout && f[0] == L"in") {
            if (!folder || f.size() != 2) return false;
        } else if (layout && (f[0] == L"app" || f[0] == L"hidden")) {
            if (f.size() != 2) return false;
            if (f[0] == L"app") folder = false;
        } else if (name == L"pins.txt" && f[0] == L"pin" && f.size() == 2) {
        } else if (name == L"icons.txt" && f[0] == L"icon" && f.size() == 3 && !f[2].empty()) {
        } else return false;
        ++rows;
    }
    return !layout || rows > 0;
}

inline bool ValidateDirectory(const std::wstring& directory, const std::wstring& skip = {}) {
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            auto name = entry.path().filename().wstring();
            if (name == skip) continue;
            if (name.starts_with(L"groups-") && !name.starts_with(L"groups-v1-")) return false;
            if (entry.is_regular_file() && entry.path().extension() == L".txt") {
                std::wstring text;
                if (!ReadUtf8(entry.path().wstring(), text) || !Validate(name, text)) return false;
            }
        }
        return true;
    } catch (...) { return false; }
}

// data-v1 is committed by a single directory rename. A partial copy or a
// failed settings conversion remains in .pending and can never look like a
// complete migration on the next start. Source files are only read/copied.
inline bool Prepare(const std::wstring& source, const std::wstring& target,
                    const std::function<bool(const std::wstring&)>& settings) {
    try {
        if (fs::exists(target)) {
            auto marker = target + L"\\ryjs-storage.ini";
            return GetPrivateProfileIntW(L"storage", L"schema", 0, marker.c_str()) == kStorageVersion &&
                   ValidateDirectory(target);
        }
        auto stage = target + L".pending";
        if (fs::exists(stage)) return false; // Keep interrupted work for inspection.
        if (!source.empty() && !ValidateDirectory(source)) return false;
        fs::create_directories(stage);
        if (!source.empty()) {
            for (const auto& entry : fs::recursive_directory_iterator(source)) {
                // Never follow junctions/symlinks or copy ephemeral writer files.
                if (entry.is_symlink() || (GetFileAttributesW(entry.path().c_str()) & FILE_ATTRIBUTE_REPARSE_POINT)) return false;
                auto relative = fs::relative(entry.path(), source);
                auto destination = fs::path(stage) / relative;
                if (entry.is_directory()) { fs::create_directories(destination); continue; }
                if (!entry.is_regular_file() || entry.path().extension() == L".tmp" ||
                    entry.path().filename().wstring().starts_with(L"performance")) continue;
                if (!CopyFileW(entry.path().c_str(), destination.c_str(), TRUE)) return false;
                // Compare source/destination bytes after copying; any racing
                // change aborts before committing the new data directory.
                auto readBytes = [](const fs::path& path) {
                    std::ifstream file(path, std::ios::binary);
                    if (!file) throw std::runtime_error("unreadable migration file");
                    return std::string(std::istreambuf_iterator<char>(file), {});
                };
                if (readBytes(entry.path()) != readBytes(destination)) return false;
            }
        }
        if (!settings(stage) || !ValidateDirectory(stage)) return false;
        auto marker = stage + L"\\ryjs-storage.ini";
        if (!WritePrivateProfileStringW(L"storage", L"schema", L"1", marker.c_str()) ||
            !WritePrivateProfileStringW(L"storage", L"source", source.c_str(), marker.c_str())) return false;
        WritePrivateProfileStringW(nullptr, nullptr, nullptr, marker.c_str());
        return MoveFileExW(stage.c_str(), target.c_str(), MOVEFILE_WRITE_THROUGH) != FALSE;
    } catch (...) { return false; }
}
} // namespace ryjs_data

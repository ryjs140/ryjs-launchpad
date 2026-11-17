#pragma once
#include <windows.h>
#include <algorithm>
#include <atomic>
#include <iterator>
#include <string>
#include <string_view>

namespace launchpad_i18n {
// English phrases are stable resource keys; stored app/folder names are never translated.
inline std::atomic<bool> chinese{false};
struct Resource { std::wstring_view english; PCWSTR zhCN; };
inline constexpr Resource resources[] = {
#include "launchpad_strings.inc"
};
inline void Select(std::wstring_view language) {
    chinese.store(language == L"zh-CN" ||
        (language != L"en" && PRIMARYLANGID(GetUserDefaultUILanguage()) == LANG_CHINESE),
        std::memory_order_relaxed);
}
inline PCWSTR Tr(PCWSTR english) {
    if (!english || !chinese.load(std::memory_order_relaxed)) return english;
    std::wstring_view key(english);
    auto p = std::lower_bound(std::begin(resources), std::end(resources), key,
        [](const Resource& r, std::wstring_view k) { return r.english < k; });
    return p != std::end(resources) && p->english == key ? p->zhCN : english;
}
inline std::wstring Text(const std::wstring& english) { return Tr(english.c_str()); }
inline PCWSTR Font() { return chinese.load(std::memory_order_relaxed) ? L"Microsoft YaHei UI" : L"Segoe UI Variable Text"; }
inline PCWSTR Locale() { return chinese.load(std::memory_order_relaxed) ? L"zh-CN" : L"en-US"; }
inline std::wstring IconFileFilter() {
    std::wstring result;
    for (PCWSTR part : {Tr(L"Pictures and icons"), L"*.ico;*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.exe", Tr(L"All files"), L"*.*"}) {
        result += part;
        result += L'\0';
    }
    result += L'\0';
    return result;
}
} // namespace launchpad_i18n

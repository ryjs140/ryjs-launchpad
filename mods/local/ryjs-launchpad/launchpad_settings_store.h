#pragma once
#include <windows.h>
#include <mutex>
#include <string>
#include <vector>

namespace launchpad_settings_store {
// Private per-mod settings, usable on Windhawk 1.x without a CLI or elevation.
// Each override carries the host value it replaced. A subsequent edit of that
// same setting in Windhawk takes precedence and invalidates the old override.
inline std::recursive_mutex mutex;
struct Change { std::wstring key, value, host; };
inline std::wstring Encode(const std::wstring& value) {
    constexpr WCHAR hex[] = L"0123456789ABCDEF";
    std::wstring out; out.reserve(value.size() * 4);
    for (WCHAR c : value) for (int shift : {12,8,4,0}) out += hex[((unsigned)c >> shift) & 15];
    return out;
}
inline bool Decode(const std::wstring& value, std::wstring& out) {
    if (value.size() % 4) return false;
    out.clear();
    unsigned code = 0;
    for (size_t i=0; i<value.size(); i++) {
        WCHAR c=value[i];
        unsigned digit=c>=L'0'&&c<=L'9' ? c-L'0' : c>=L'A'&&c<=L'F' ? c-L'A'+10 : 99;
        if (digit>15) return false;
        code=(code<<4)|digit;
        if (i%4==3) { out+=(WCHAR)code; code=0; }
    }
    return true;
}
inline std::wstring Read(const std::wstring& path, PCWSTR section, PCWSTR key) {
    std::wstring result(1024,L'\0');
    while (result.size() <= 1024*1024) {
        DWORD n=GetPrivateProfileStringW(section,key,L"",result.data(),(DWORD)result.size(),path.c_str());
        if (n+1<result.size()) { result.resize(n); return result; }
        result.resize(result.size()*2);
    }
    return {};
}
inline std::wstring Resolve(const std::wstring& path, PCWSTR key, const std::wstring& host) {
    std::lock_guard lock(mutex);
    if (GetPrivateProfileIntW(L"meta",L"version",1,path.c_str())>1 || Read(path,L"present",key)!=L"1") return host;
    std::wstring baseline,value;
    if (!Decode(Read(path,L"host",key),baseline) || !Decode(Read(path,L"values",key),value)) return host;
    if (baseline!=host) {
        WritePrivateProfileStringW(L"present",key,nullptr,path.c_str());
        return host;
    }
    return value;
}
// The read-only sibling of Resolve, used when a private store is examined
// without being invalidated. The mod no longer inherits another mod's settings,
// but dev/test_storage.cpp still checks the inheritance semantics through it,
// so it stays part of the test interface.
inline std::wstring ResolveReadOnly(const std::wstring& path, PCWSTR key, const std::wstring& host) {
    std::lock_guard lock(mutex);
    if (GetPrivateProfileIntW(L"meta",L"version",1,path.c_str()) != 1 || Read(path,L"present",key)!=L"1") return host;
    std::wstring baseline, value;
    return Decode(Read(path,L"host",key),baseline) && Decode(Read(path,L"values",key),value) && baseline==host ? value : host;
}
inline bool Save(const std::wstring& path, const std::vector<Change>& changes) {
    std::lock_guard lock(mutex);
    if (GetPrivateProfileIntW(L"meta",L"version",1,path.c_str())>1) return false;
    std::wstring temporary=path+L".tmp";
    bool existing=GetFileAttributesW(path.c_str())!=INVALID_FILE_ATTRIBUTES;
    if (existing) { if (!CopyFileW(path.c_str(),temporary.c_str(),FALSE)) return false; }
    else {
        HANDLE file=CreateFileW(temporary.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
        if (file==INVALID_HANDLE_VALUE) return false;
        WORD bom=0xFEFF; DWORD written; bool ok=WriteFile(file,&bom,2,&written,nullptr)&&written==2;
        CloseHandle(file); if (!ok) return false;
    }
    bool ok=WritePrivateProfileStringW(L"meta",L"version",L"1",temporary.c_str())!=FALSE;
    for (const auto& change : changes) {
        ok=WritePrivateProfileStringW(L"host",change.key.c_str(),Encode(change.host).c_str(),temporary.c_str())&&ok;
        ok=WritePrivateProfileStringW(L"values",change.key.c_str(),Encode(change.value).c_str(),temporary.c_str())&&ok;
        ok=WritePrivateProfileStringW(L"present",change.key.c_str(),L"1",temporary.c_str())&&ok;
    }
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,temporary.c_str());
    if (existing && !CopyFileW(path.c_str(),(path+L".bak").c_str(),FALSE)) ok=false;
    if (ok) ok=MoveFileExW(temporary.c_str(),path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)!=FALSE;
    if (!ok) DeleteFileW(temporary.c_str());
    // Discard the profile cache after an atomic file replacement.
    WritePrivateProfileStringW(nullptr,nullptr,nullptr,path.c_str());
    return ok;
}
} // namespace launchpad_settings_store

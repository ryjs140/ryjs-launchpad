// Stable identities keep edits independent of automatic discovery and layout order.
struct EntryRecord {
    bool manual=false;
    std::wstring target,name,arguments,directory;
};
[[clang::no_destroy]] std::map<std::wstring,EntryRecord> g_entries;
bool g_entriesReadOnly=false;

std::wstring EntryEncode(const std::wstring& value) {
    std::wstring result;
    for (auto c:value) {
        if (c==L'%') result+=L"%25";
        else if (c==L'\t') result+=L"%09";
        else if (c==L'\n') result+=L"%0A";
        else if (c==L'\r') result+=L"%0D";
        else result+=c;
    }
    return result;
}
bool EntryDecode(const std::wstring& value,std::wstring& result) {
    result.clear();
    for (size_t i=0;i<value.size();i++) {
        if (value[i]!=L'%') { result+=value[i]; continue; }
        auto escape=value.substr(i,3);
        if (escape==L"%25") result+=L'%';
        else if (escape==L"%09") result+=L'\t';
        else if (escape==L"%0A") result+=L'\n';
        else if (escape==L"%0D") result+=L'\r';
        else return false;
        i+=2;
    }
    return true;
}
bool SaveEntries() {
    if (g_entriesReadOnly) return false;
    std::vector<std::wstring> lines{L"schema\t1\tentries"};
    for (const auto& [key,e]:g_entries) lines.push_back(L"entry\t"+EntryEncode(key)+L"\t"+(e.manual?L"1":L"0")+
        L"\t"+EntryEncode(e.target)+L"\t"+EntryEncode(e.name)+L"\t"+EntryEncode(e.arguments)+L"\t"+EntryEncode(e.directory));
    return WriteLines(StoragePath(L"entries-v1.txt"),lines);
}
bool LoadEntries() {
    auto path=StoragePath(L"entries-v1.txt");
    bool read=false;
    auto rows=ReadLines(path,&read);
    bool missing=GetFileAttributesW(path.c_str())==INVALID_FILE_ATTRIBUTES && GetLastError()==ERROR_FILE_NOT_FOUND;
    if (missing) { g_entries.clear(); g_entriesReadOnly=false; return true; }
    std::map<std::wstring,EntryRecord> parsed;
    bool valid=read && !rows.empty() && rows[0]==std::vector<std::wstring>{L"schema",L"1",L"entries"};
    for (size_t i=1;valid && i<rows.size();i++) {
        auto& r=rows[i]; std::wstring key; EntryRecord e;
        valid=r.size()==7 && r[0]==L"entry" && (r[2]==L"0"||r[2]==L"1");
        if (!valid) break;
        e.manual=r[2]==L"1";
        valid=EntryDecode(r[1],key) && EntryDecode(r[3],e.target) && EntryDecode(r[4],e.name) &&
            EntryDecode(r[5],e.arguments) && EntryDecode(r[6],e.directory) && !key.empty() && !e.target.empty() && !e.name.empty();
        if (valid) valid=parsed.emplace(key,std::move(e)).second;
    }
    g_entriesReadOnly=!valid;
    if (valid) g_entries=std::move(parsed);
    else Wh_Log(L"Entry data is unreadable or unsupported; keeping it read-only.");
    return valid;
}
bool IsWebEntry(const std::wstring& target) {
    auto lower=ToLower(target);
    return lower.starts_with(L"https://") || lower.starts_with(L"http://");
}
std::wstring WebsiteUrlFromArguments(const std::wstring& arguments) {
    if (arguments.empty()) return {};
    int count = 0;
    auto command = L"browser.exe " + arguments;
    auto argv = CommandLineToArgvW(command.c_str(), &count);
    if (!argv) return {};
    std::wstring url;
    for (int i = 1; i < count; ++i) {
        std::wstring argument = argv[i];
        if (argument.starts_with(L"--app=")) argument.erase(0, 6);
        else if (count != 2) continue;
        if (IsWebEntry(argument)) {
            url = std::move(argument);
            break;
        }
    }
    LocalFree(argv);
    return url;
}

std::wstring ResolveWebsiteUrl(const App& app) {
    if (IsWebEntry(app.target)) return app.target;
    auto target = ToLower(app.target);
    WCHAR value[32768]{};
    if (EndsWith(target, L".url") && !PathIsRelativeW(app.target.c_str())) {
        DWORD length = GetPrivateProfileStringW(L"InternetShortcut", L"URL", L"", value, ARRAYSIZE(value), app.target.c_str());
        if (length < (DWORD)ARRAYSIZE(value) - 1 && IsWebEntry(value)) return value;
    } else if (EndsWith(target, L".exe")) {
        return WebsiteUrlFromArguments(app.arguments);
    } else if (EndsWith(target, L".lnk")) {
        ComPtr<IShellLinkW> link;
        ComPtr<IPersistFile> persist;
        if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&link))) &&
            SUCCEEDED(link.As(&persist)) && SUCCEEDED(persist->Load(app.target.c_str(), STGM_READ)) &&
            SUCCEEDED(link->GetArguments(value, ARRAYSIZE(value)))) {
            return WebsiteUrlFromArguments(value);
        }
    }
    return {};
}
bool IsEntryMissing(const App& app) {
    if (IsWebEntry(app.target) || ToLower(app.target).starts_with(L"shell:")) return false;
    return GetFileAttributesW(app.target.c_str())==INVALID_FILE_ATTRIBUTES;
}
std::wstring WebIconPath(const std::wstring& key) {
    unsigned long long hash=14695981039346656037ull;
    for (WCHAR c:key) { hash^=c;hash*=1099511628211ull; }
    WCHAR name[40]{};swprintf_s(name,L"web-icons\\%016llx.ico",hash);
    return StoragePath(name);
}
bool LoadWebsiteIcon(int size, App& app) {
    auto path = WebIconPath(AppKey(app) + L"|" + app.websiteUrl);
    if (GetFileAttributesW(path.c_str()) == INVALID_FILE_ATTRIBUTES) return false;
    // A missing/corrupt cache must not erase the shell icon already loaded.
    App decoded;
    if (!LoadIconFile(path, size, decoded, false)) return false;
    app.pixels = std::move(decoded.pixels);
    app.iconSize = decoded.iconSize;
    app.bitmap.Reset();
    app.generation = 0;
    return true;
}

void ReloadEntryIcon(int size, App& app) {
    if (app.websiteUrl.empty()) ReloadAppIcon(size, app);
    else LoadWebsiteIcon(size, app);
}

void ApplyEntryRecord(App& app,const std::wstring& key,const EntryRecord& e,int size) {
    if (app.originalName.empty()) app.originalName=app.name;
    if(app.layoutTarget.empty())app.layoutTarget=app.target.empty()?key:app.target;
    app.stableKey=key; app.manual=e.manual; app.name=e.name;
    app.target=e.target; app.arguments=e.arguments; app.workingDirectory=e.directory;
    app.unavailable=IsEntryMissing(app);
    app.websiteUrl=ResolveWebsiteUrl(app);
    ReloadEntryIcon(size,app);
}
void ApplyEntries(int size) {
    if (!LoadEntries()) return;
    for (auto& app:g_apps) app->originalName=app->name;
    for (const auto& [key,e]:g_entries) {
        auto found=std::find_if(g_apps.begin(),g_apps.end(),[&](const auto& a) {return AppKey(*a)==key;});
        if (found!=g_apps.end()) ApplyEntryRecord(**found,key,e,size);
        else {
            // Keep a modified automatic app too, so a missing target can be repaired.
            auto app=std::make_shared<App>(); app->name=e.name;
            ApplyEntryRecord(*app,key,e,size); g_apps.push_back(app);
        }
    }
}

// Launchpad-only names, keyed by AppKey, never by the visible label. No shell,
// shortcut or registry write is part of this feature.
struct DisplayNameRecord {std::wstring original,custom;};
[[clang::no_destroy]] std::map<std::wstring,DisplayNameRecord> g_displayNames;
bool g_displayNamesReadOnly=false;
bool LoadDisplayNames() {
    auto path=StoragePath(L"display-names-v1.txt");bool read=false;auto rows=ReadLines(path,&read);
    if(GetFileAttributesW(path.c_str())==INVALID_FILE_ATTRIBUTES&&GetLastError()==ERROR_FILE_NOT_FOUND) {
        g_displayNames.clear();g_displayNamesReadOnly=false;return true;
    }
    bool valid=read&&!rows.empty()&&rows[0]==std::vector<std::wstring>{L"schema",L"1",L"display-names"};
    std::map<std::wstring,DisplayNameRecord> parsed;
    for(size_t i=1;valid&&i<rows.size();i++) {
        const auto& r=rows[i];std::wstring key;DisplayNameRecord record;
        valid=r.size()==4&&r[0]==L"name"&&EntryDecode(r[1],key)&&EntryDecode(r[2],record.original)&&
            EntryDecode(r[3],record.custom)&&!key.empty()&&!record.original.empty();
        if(valid)valid=parsed.emplace(key,record).second;
    }
    g_displayNamesReadOnly=!valid;if(valid)g_displayNames=std::move(parsed);
    return valid;
}
bool SaveDisplayNames() {
    if(g_displayNamesReadOnly||g_dataReadOnly)return false;
    std::vector<std::wstring> lines{L"schema\t1\tdisplay-names"};
    for(const auto& [key,r]:g_displayNames)lines.push_back(L"name\t"+EntryEncode(key)+L"\t"+EntryEncode(r.original)+L"\t"+EntryEncode(r.custom));
    return WriteLines(StoragePath(L"display-names-v1.txt"),lines);
}
void ApplyDisplayName(App& app) {
    auto found=g_displayNames.find(AppKey(app));if(found==g_displayNames.end())return;
    // Preserve the source name separately, including for missing/manual entries.
    if(app.manual||app.originalName.empty()||(app.unavailable&&app.originalName==app.name))app.originalName=found->second.original;
    app.name=found->second.custom.empty()?app.originalName:found->second.custom;
}
void ApplyDisplayNames() {
    if(!LoadDisplayNames())return;
    for(auto& app:g_apps)ApplyDisplayName(*app);
}
bool SetDisplayName(const std::shared_ptr<App>& app,const std::wstring& name) {
    if(!app||g_displayNamesReadOnly||g_dataReadOnly)return false;
    if(!name.empty()&&(name.size()>160||name.find_first_not_of(L" ")==std::wstring::npos||
        std::any_of(name.begin(),name.end(),[](WCHAR c){return c<32;})))return false;
    auto key=AppKey(*app);auto before=g_displayNames;
    std::wstring original=app->originalName.empty()?app->name:app->originalName;
    auto found=g_displayNames.find(key);
    if(found!=g_displayNames.end()&&(app->manual||app->originalName.empty()))original=found->second.original;
    g_displayNames[key]={original,name};
    if(!SaveDisplayNames()){g_displayNames=std::move(before);return false;}
    app->originalName=original;app->name=name.empty()?original:name;
    return true;
}

// Fetch only for an explicit add/update. No periodic networking or third-party proxy.
struct WebHandle {
    HINTERNET value=nullptr;
    WebHandle(HINTERNET h=nullptr):value(h){}
    ~WebHandle(){if(value)WinHttpCloseHandle(value);}
    operator HINTERNET() const{return value;}
};
struct WebIconResult {std::wstring key,url,path;};
[[clang::no_destroy]] std::vector<HANDLE> g_webWorkers;
std::atomic<bool> g_webStop=false;
constexpr UINT kWebIconReady=WM_APP+64;
std::wstring WebWide(const std::string& bytes) {
    int n=MultiByteToWideChar(CP_UTF8,0,bytes.data(),(int)bytes.size(),nullptr,0);
    std::wstring value(n,L'\0');if(n)MultiByteToWideChar(CP_UTF8,0,bytes.data(),(int)bytes.size(),value.data(),n);return value;
}
bool DownloadWeb(const std::wstring& url,std::string& bytes,std::wstring& finalUrl) {
    if (g_webStop || !IsWebEntry(url)) return false;
    URL_COMPONENTS parts{sizeof(parts)};
    parts.dwHostNameLength=parts.dwUrlPathLength=parts.dwExtraInfoLength=parts.dwUserNameLength=parts.dwPasswordLength=(DWORD)-1;
    if (!WinHttpCrackUrl(url.c_str(),0,0,&parts) || !parts.dwHostNameLength || parts.dwUserNameLength || parts.dwPasswordLength) return false;
    std::wstring host(parts.lpszHostName,parts.dwHostNameLength),path(parts.lpszUrlPath,parts.dwUrlPathLength);
    path+=std::wstring(parts.lpszExtraInfo?parts.lpszExtraInfo:L"",parts.dwExtraInfoLength);
    if(path.empty())path=L"/";
    WebHandle session(WinHttpOpen(L"RYJSLaunchpad/0.1",WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0));
    if(!session)return false;
    WinHttpSetTimeouts(session,3000,3000,3000,3000);
    WebHandle connection(WinHttpConnect(session,host.c_str(),parts.nPort,0));if(!connection)return false;
    WebHandle request(WinHttpOpenRequest(connection,L"GET",path.c_str(),nullptr,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,parts.nScheme==INTERNET_SCHEME_HTTPS?WINHTTP_FLAG_SECURE:0));
    if(!request)return false;
    DWORD disabled=WINHTTP_DISABLE_COOKIES;WinHttpSetOption(request,WINHTTP_OPTION_DISABLE_FEATURE,&disabled,sizeof(disabled));
    DWORD logon=WINHTTP_AUTOLOGON_SECURITY_LEVEL_HIGH;WinHttpSetOption(request,WINHTTP_OPTION_AUTOLOGON_POLICY,&logon,sizeof(logon));
    DWORD policy=WINHTTP_OPTION_REDIRECT_POLICY_DISALLOW_HTTPS_TO_HTTP;WinHttpSetOption(request,WINHTTP_OPTION_REDIRECT_POLICY,&policy,sizeof(policy));
    DWORD redirects=5;WinHttpSetOption(request,WINHTTP_OPTION_MAX_HTTP_AUTOMATIC_REDIRECTS,&redirects,sizeof(redirects));
    if(!WinHttpSendRequest(request,WINHTTP_NO_ADDITIONAL_HEADERS,0,WINHTTP_NO_REQUEST_DATA,0,0,0)||!WinHttpReceiveResponse(request,nullptr))return false;
    DWORD code=0,size=sizeof(code);
    if(!WinHttpQueryHeaders(request,WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER,nullptr,&code,&size,nullptr)||code!=200)return false;
    bytes.clear();char buffer[16384];DWORD count=0;
    ULONGLONG began=GetTickCount64();
    while(!g_webStop) {
        if(GetTickCount64()-began>15000)return false;
        if(!WinHttpReadData(request,buffer,sizeof(buffer),&count))return false;
        if(!count)break;
        if(bytes.size()+count>2*1024*1024)return false;
        bytes.append(buffer,count);
    }
    if(g_webStop||bytes.empty())return false;
    DWORD needed=0;WinHttpQueryOption(request,WINHTTP_OPTION_URL,nullptr,&needed);
    finalUrl.resize(needed/sizeof(WCHAR));
    if(needed&&WinHttpQueryOption(request,WINHTTP_OPTION_URL,finalUrl.data(),&needed))finalUrl.resize(wcslen(finalUrl.c_str()));
    else finalUrl=url;
    return true;
}
std::wstring WebAttribute(const std::wstring& tag,const std::wstring& name) {
    // Require an attribute boundary so data-href is not mistaken for href.
    std::wregex pattern(L"(?:^|\\s)"+name+LR"attr(\s*=\s*(?:"([^"]*)"|'([^']*)'|([^\s>]+)))attr",std::regex::icase);
    std::wsmatch match;
    if(!std::regex_search(tag,match,pattern))return {};
    std::wstring value=match[1].matched?match[1].str():match[2].matched?match[2].str():match[3].str();
    for(size_t pos=0;(pos=value.find(L"&amp;",pos))!=std::wstring::npos;)value.replace(pos,5,L"&");
    return value;
}
std::vector<std::wstring> WebsiteIconCandidates(const std::wstring& html,const std::wstring& base) {
    std::vector<std::wstring> result;
    auto resolve=[&](const std::wstring& relative) {
        WCHAR url[32768]{};
        DWORD n=ARRAYSIZE(url);
        if(SUCCEEDED(UrlCombineW(base.c_str(),relative.c_str(),url,&n,0))&&IsWebEntry(url)&&std::find(result.begin(),result.end(),url)==result.end())
            result.emplace_back(url);
    };
    std::wregex links(LR"(<link\b[^>]{0,4096}>)",std::regex::icase);
    for(std::wsregex_iterator it(html.begin(),html.end(),links),end;it!=end&&result.size()<5;++it) {
        auto tag=it->str(),rel=ToLower(WebAttribute(tag,L"rel"));
        if(rel.find(L"icon")!=std::wstring::npos)resolve(WebAttribute(tag,L"href"));
    }
    resolve(L"/favicon.ico");return result;
}
DWORD WINAPI WebIconWorker(void* argument) {
    std::unique_ptr<WebIconResult> result((WebIconResult*)argument);
    CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    std::string bytes;std::wstring base;
    std::vector<std::wstring> candidates;
    if(DownloadWeb(result->url,bytes,base))candidates=WebsiteIconCandidates(WebWide(bytes),base);
    else candidates=WebsiteIconCandidates(L"",result->url);
    auto temp=result->path+L"."+std::to_wstring(GetCurrentThreadId())+L".download";
    for(const auto& url:candidates) {
        if(g_webStop)break;
        if(!DownloadWeb(url,bytes,base))continue;
        HANDLE file=CreateFileW(temp.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
        if(file==INVALID_HANDLE_VALUE)continue;
        DWORD written=0;bool saved=WriteFile(file,bytes.data(),(DWORD)bytes.size(),&written,nullptr)&&written==bytes.size();CloseHandle(file);
        App decoded;
        ComPtr<IWICImagingFactory> factory;ComPtr<IWICBitmapDecoder> decoder;
        bool image=saved&&SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory)))&&
            SUCCEEDED(factory->CreateDecoderFromFilename(temp.c_str(),nullptr,GENERIC_READ,WICDecodeMetadataCacheOnLoad,&decoder));
        bool decodedImage=image?LoadIconFile(temp,128,decoded):LoadSvgIcon(temp,128,decoded);
        if(decodedImage && MoveFileExW(temp.c_str(),result->path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)) {
            HWND window=g_wnd;
            if(!g_webStop&&window&&PostMessageW(window,kWebIconReady,0,(LPARAM)result.get()))result.release();
            break;
        }
        DeleteFileW(temp.c_str());
    }
    DeleteFileW(temp.c_str());CoUninitialize();return 0;
}
void StartWebIconFetch(const std::wstring& key,const std::wstring& url) {
    if(g_webStop || !IsWebEntry(url))return;
    for(auto it=g_webWorkers.begin();it!=g_webWorkers.end();) {
        if(WaitForSingleObject(*it,0)==WAIT_OBJECT_0) {
            CloseHandle(*it);
            it=g_webWorkers.erase(it);
        } else ++it;
    }
    if(g_webWorkers.size()>=4)return;
    auto result=std::make_unique<WebIconResult>();result->key=key;result->url=url;result->path=WebIconPath(key+L"|"+url);
    SHCreateDirectoryExW(nullptr,std::filesystem::path(result->path).parent_path().c_str(),nullptr);
    HANDLE worker=CreateThread(nullptr,0,WebIconWorker,result.get(),0,nullptr);
    if(worker){g_webWorkers.push_back(worker);result.release();}
}
void OnWebIconReady(WebIconResult* payload) {
    std::unique_ptr<WebIconResult> result(payload);
    if(g_customIcons.count(result->key))return;
    for(const auto& app:g_apps)if(AppKey(*app)==result->key&&app->target==result->url){LoadIconFile(result->path,IconPixels(),*app);Render();break;}
}
void StopWebIcons() {
    g_webStop=true;
    for(HANDLE worker:g_webWorkers){WaitForSingleObject(worker,INFINITE);CloseHandle(worker);}g_webWorkers.clear();
    MSG message{};while(PeekMessageW(&message,nullptr,kWebIconReady,kWebIconReady,PM_REMOVE))delete (WebIconResult*)message.lParam;
}

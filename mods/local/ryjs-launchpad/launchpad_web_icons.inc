// Fill missing website icons on discovery and on explicit refresh.
struct WebHandle {
    HINTERNET value=nullptr;
    WebHandle(HINTERNET h=nullptr):value(h){}
    ~WebHandle(){if(value)WinHttpCloseHandle(value);}
    operator HINTERNET() const{return value;}
};
struct WebIconResult {
    std::wstring key,url,path;
    DWORD threadId=0, accessStatus=0;
    bool reportFailure=false, saved=false;
};
[[clang::no_destroy]] std::vector<HANDLE> g_webWorkers;
[[clang::no_destroy]] std::deque<WebIconResult> g_pendingWebIcons;
[[clang::no_destroy]] std::set<std::wstring> g_attemptedWebIcons;
std::atomic<bool> g_webStop=false;
constexpr UINT kWebIconReady=WM_APP+64;
constexpr size_t kMaxWebIconBytes=2*1024*1024;
std::wstring WebWide(const std::string& bytes) {
    int n=MultiByteToWideChar(CP_UTF8,0,bytes.data(),(int)bytes.size(),nullptr,0);
    std::wstring value(n,L'\0');if(n)MultiByteToWideChar(CP_UTF8,0,bytes.data(),(int)bytes.size(),value.data(),n);return value;
}
bool DownloadWeb(const std::wstring& url,std::string& bytes,std::wstring& finalUrl,DWORD& status) {
    status=0;
    if (g_webStop || !IsWebEntry(url)) return false;
    URL_COMPONENTS parts{sizeof(parts)};
    parts.dwHostNameLength=parts.dwUrlPathLength=parts.dwExtraInfoLength=parts.dwUserNameLength=parts.dwPasswordLength=(DWORD)-1;
    if (!WinHttpCrackUrl(url.c_str(),0,0,&parts) || !parts.dwHostNameLength || parts.dwUserNameLength || parts.dwPasswordLength) return false;
    std::wstring host(parts.lpszHostName,parts.dwHostNameLength),path(parts.lpszUrlPath,parts.dwUrlPathLength);
    path+=std::wstring(parts.lpszExtraInfo?parts.lpszExtraInfo:L"",parts.dwExtraInfoLength);
    if(auto fragment=path.find(L'#');fragment!=std::wstring::npos)path.resize(fragment);
    if(path.empty()||path.front()==L'?')path.insert(path.begin(),L'/');
    WebHandle session(WinHttpOpen(L"Lumiflow/" WH_MOD_VERSION,WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0));
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
    DWORD size=sizeof(status);
    if(!WinHttpQueryHeaders(request,WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER,nullptr,&status,&size,nullptr)||status!=200)return false;
    bytes.clear();char buffer[16384];DWORD count=0;
    ULONGLONG began=GetTickCount64();
    while(!g_webStop) {
        if(GetTickCount64()-began>15000)return false;
        if(!WinHttpReadData(request,buffer,sizeof(buffer),&count))return false;
        if(!count)break;
        if(bytes.size()+count>kMaxWebIconBytes)return false;
        bytes.append(buffer,count);
    }
    if(g_webStop||bytes.empty())return false;
    DWORD needed=0;WinHttpQueryOption(request,WINHTTP_OPTION_URL,nullptr,&needed);
    finalUrl.resize(needed/sizeof(WCHAR));
    if(needed&&WinHttpQueryOption(request,WINHTTP_OPTION_URL,finalUrl.data(),&needed))finalUrl.resize(wcslen(finalUrl.c_str()));
    else finalUrl=url;
    return true;
}
bool DecodeInlineWebsiteIcon(std::wstring_view url,std::string& bytes) {
    auto comma=url.find(L',');
    if(comma==url.npos||comma>256||url.size()>kMaxWebIconBytes)return false;
    auto type=ToLower(std::wstring(url.substr(0,comma)));
    if(!type.starts_with(L"data:image/"))return false;
    auto content=url.substr(comma+1);
    if(content.empty())return false;
    int size=WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,content.data(),(int)content.size(),nullptr,0,nullptr,nullptr);
    if(size<=0||(size_t)size>kMaxWebIconBytes)return false;
    std::string encoded(size,'\0');
    WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,content.data(),(int)content.size(),encoded.data(),size,nullptr,nullptr);
    bytes.clear();
    auto hex=[](char c) {
        if(c>='0'&&c<='9')return c-'0';
        if(c>='A'&&c<='F')return c-'A'+10;
        if(c>='a'&&c<='f')return c-'a'+10;
        return -1;
    };
    for(size_t i=0;i<encoded.size();++i) {
        if(encoded[i]!='%') {bytes+=encoded[i];continue;}
        if(i+2>=encoded.size())return false;
        int high=hex(encoded[i+1]),low=hex(encoded[i+2]);
        if(high<0||low<0)return false;
        bytes+=static_cast<char>(high*16+low);
        i+=2;
    }
    if(!type.ends_with(L";base64"))return !bytes.empty();

    constexpr std::string_view alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    auto base64=std::move(bytes);
    bytes.clear();
    const auto padding=base64.find('=');
    if(padding!=base64.npos) {
        if(base64.size()%4||base64.size()-padding>2||base64.find_first_not_of('=',padding)!=base64.npos)return false;
        base64.resize(padding);
    }
    if(base64.size()%4==1)return false;
    unsigned buffer=0,bits=0;
    for(char c:base64) {
        auto digit=alphabet.find(c);
        if(digit==alphabet.npos)return false;
        buffer=(buffer<<6)|static_cast<unsigned>(digit);
        bits+=6;
        if(bits>=8) {
            bits-=8;
            bytes+=static_cast<char>((buffer>>bits)&0xff);
        }
    }
    return !bytes.empty() && !(buffer & ((1u<<bits)-1));
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
std::wstring WebsiteTag(const std::wstring& html,size_t begin) {
    size_t end=begin;
    while((end=html.find_first_of(L"\"'>",end))!=html.npos) {
        if(html[end]==L'>')return html.substr(begin,end-begin+1);
        end=html.find(html[end],end+1);
        if(end==html.npos)break;
        ++end;
    }
    return {};
}
std::vector<std::wstring> WebsiteIconCandidates(const std::wstring& html,const std::wstring& base) {
    std::vector<std::wstring> result;
    auto resolve=[](const std::wstring& relative,const std::wstring& origin,bool inlineIcon=false) -> std::wstring {
        auto first=relative.find_first_not_of(L" \t\r\n");
        if(first==std::wstring::npos)return {};
        auto href=relative.substr(first,relative.find_last_not_of(L" \t\r\n")-first+1);
        if(inlineIcon&&ToLower(href.substr(0,11))==L"data:image/")return href;
        WCHAR url[32768]{};
        DWORD n=ARRAYSIZE(url);
        if(SUCCEEDED(UrlCombineW(origin.c_str(),href.c_str(),url,&n,0))&&IsWebEntry(url))return url;
        return {};
    };
    auto add=[&](std::wstring url) {
        if(!url.empty()&&std::find(result.begin(),result.end(),url)==result.end())result.push_back(std::move(url));
    };
    static const std::wregex bases(LR"(<base\s)",std::regex::icase);
    static const std::wregex links(LR"(<link\s)",std::regex::icase);
    static const std::wregex iconRel(LR"((?:^|\s)(?:icon|apple-touch-icon|apple-touch-icon-precomposed|mask-icon)(?:\s|$))");
    std::wsmatch baseTag;
    auto origin=base;
    if(std::regex_search(html,baseTag,bases)) {
        auto declared=resolve(WebAttribute(WebsiteTag(html,baseTag.position()),L"href"),base);
        if(!declared.empty())origin=std::move(declared);
    }
    std::vector<std::wstring> secondary;
    for(std::wsregex_iterator it(html.begin(),html.end(),links),end;it!=end&&result.size()<5;++it) {
        auto tag=WebsiteTag(html,it->position()),rel=ToLower(WebAttribute(tag,L"rel"));
        if(!std::regex_search(rel,iconRel))continue;
        auto url=resolve(WebAttribute(tag,L"href"),origin,true);
        if(rel.starts_with(L"apple-touch-icon")||rel==L"mask-icon") {
            if(secondary.size()<3)secondary.push_back(std::move(url));
        } else add(std::move(url));
    }
    for(auto& url:secondary)add(std::move(url));
    add(resolve(L"/favicon.ico",base));
    return result;
}
DWORD WINAPI WebIconWorker(void* argument) {
    std::unique_ptr<WebIconResult> result((WebIconResult*)argument);
    result->threadId=GetCurrentThreadId();
    CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);
    std::string bytes;std::wstring base;
    std::vector<std::wstring> candidates;
    DWORD status=0;
    if(DownloadWeb(result->url,bytes,base,status))candidates=WebsiteIconCandidates(WebWide(bytes),base);
    else {
        if(status==401||status==403)result->accessStatus=status;
        candidates=WebsiteIconCandidates(L"",result->url);
    }
    auto temp=result->path+L"."+std::to_wstring(result->threadId)+L".download";
    for(const auto& url:candidates) {
        if(g_webStop)break;
        if(ToLower(url.substr(0,11))==L"data:image/") {
            if(!DecodeInlineWebsiteIcon(url,bytes))continue;
        } else if(!DownloadWeb(url,bytes,base,status)) {
            if(status==401||status==403)result->accessStatus=status;
            continue;
        }
        HANDLE file=CreateFileW(temp.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
        if(file==INVALID_HANDLE_VALUE)continue;
        DWORD written=0;bool saved=WriteFile(file,bytes.data(),(DWORD)bytes.size(),&written,nullptr)&&written==bytes.size();CloseHandle(file);
        App decoded;
        if(saved && LoadIconFile(temp,128,decoded,false) &&
           MoveFileExW(temp.c_str(),result->path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)) {
            result->saved=true;
            break;
        }
        DeleteFileW(temp.c_str());
    }
    DeleteFileW(temp.c_str());
    CoUninitialize();
    HWND window=g_wnd;
    if(!g_webStop&&window&&PostMessageW(window,kWebIconReady,0,(LPARAM)result.get()))result.release();
    return 0;
}
void PumpWebIcons() {
    if(g_webStop)return;
    for(auto it=g_webWorkers.begin();it!=g_webWorkers.end();) {
        if(WaitForSingleObject(*it,0)==WAIT_OBJECT_0) {
            CloseHandle(*it);
            it=g_webWorkers.erase(it);
        } else ++it;
    }
    while(g_webWorkers.size()<4&&!g_pendingWebIcons.empty()) {
        auto result=std::make_unique<WebIconResult>(std::move(g_pendingWebIcons.front()));
        g_pendingWebIcons.pop_front();
        auto payload=result.release();
        HANDLE worker=CreateThread(nullptr,0,WebIconWorker,payload,0,nullptr);
        if(worker)g_webWorkers.push_back(worker);
        else delete payload;
    }
}
void StartWebIconFetch(const App& app,bool refresh=false,bool reportFailure=false) {
    if(g_webStop || g_dataReadOnly || app.websiteUrl.empty())return;
    auto key=AppKey(app);
    auto path=WebIconPath(key+L"|"+app.websiteUrl);
    if(path.empty()||(!g_attemptedWebIcons.insert(path).second&&!refresh))return;
    SHCreateDirectoryExW(nullptr,std::filesystem::path(path).parent_path().c_str(),nullptr);
    g_pendingWebIcons.push_back({std::move(key),app.websiteUrl,std::move(path),0,0,reportFailure});
    PumpWebIcons();
}
void OnWebIconReady(WebIconResult* payload) {
    std::unique_ptr<WebIconResult> result(payload);
    // Result ownership transferred with the message; never resolve a worker by
    // a reusable thread ID or block the UI waiting for a thread to exit.
    PumpWebIcons();
    if(g_customIcons.count(result->key))return;
    for(const auto& app:g_apps)if(AppKey(*app)==result->key&&app->websiteUrl==result->url) {
        if(result->saved&&LoadIconFile(result->path,IconPixels(),*app,false))Render();
        else if(result->reportFailure&&g_shown&&!g_closing&&!g_modal) {
            auto message=std::wstring(Tr(L"Could not load a website icon."));
            if(result->accessStatus)
                message=std::wstring(Tr(L"The website denied access to the icon."))+L" (HTTP "+std::to_wstring(result->accessStatus)+L")";
            g_modal=true;
            MessageBoxW(g_wnd,message.c_str(),Tr(L"Refresh website icon"),MB_OK|MB_ICONINFORMATION);
            g_modal=false;
        }
        break;
    }
}
void StopWebIcons() {
    g_webStop=true;
    g_pendingWebIcons.clear();g_attemptedWebIcons.clear();
    for(HANDLE worker:g_webWorkers){WaitForSingleObject(worker,INFINITE);CloseHandle(worker);}g_webWorkers.clear();
    MSG message{};while(PeekMessageW(&message,nullptr,kWebIconReady,kWebIconReady,PM_REMOVE))delete (WebIconResult*)message.lParam;
}


// Windows 11 Start is a XAML ExperienceToggleButton, not a Win32 "Start"
// child window. Its OnToggle latches IsChecked before Click reaches our
// ShowStartView hook. Intercept that toggle when replacing the native menu;
// otherwise no native menu-close notification ever clears the checked state.
// Hook the WinRT ABI setter: pThis is an actual COM interface, so identifying
// the element requires no implementation offsets or foreign memory writes.
// The checked-state latch was diagnosed with this trace. Writing it runs on
// the taskbar's UI thread on every press, release and pointer move, so it is
// compiled out of normal builds; set this to true only for that diagnosis.
constexpr bool kStartStateTrace = false;
void StartStateTrace(PCWSTR event,int checked=-2,int pressed=-2,int hover=-2,int focus=-2) {
    if(!kStartStateTrace)return;
    auto path=StoragePath(L"start-state-trace.log");
    HANDLE file=CreateFileW(path.c_str(),FILE_APPEND_DATA,FILE_SHARE_READ|FILE_SHARE_WRITE,nullptr,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(file==INVALID_HANDLE_VALUE)return;
    LARGE_INTEGER size{};if(GetFileSizeEx(file,&size)&&size.QuadPart<(1<<20)) {
        char row[560];int n=snprintf(row,sizeof(row),"%llu %ls checked=%d pressed=%d hover=%d replacement=%d native=%d focus=%d\r\n",
            GetTickCount64(),event,checked,pressed,hover,(int)g_startButtonReplacementActive.load(),(int)g_nativeStartPassthrough.load(),focus);
        if(n>0&&n<(int)sizeof(row)){DWORD written;WriteFile(file,row,n,&written,nullptr);}
    }
    CloseHandle(file);
}
// Remember the element when the public toggle or visibility setter sees a
// replaced Start request. XAML objects live on the taskbar's UI thread, so
// the launcher posts its cleanup through the element's own dispatcher.
using StartButtonWeak = winrt::weak_ref<winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton>;
using StartDispatcher = winrt::Windows::UI::Core::CoreDispatcher;
struct StartButtonReference {
    StartButtonWeak element;
    StartDispatcher dispatcher{nullptr};
};
[[clang::no_destroy]] static StartButtonReference g_startButtonReference;
SRWLOCK g_startButtonElementLock = SRWLOCK_INIT;
struct StartButtonLock {
    StartButtonLock() { AcquireSRWLockExclusive(&g_startButtonElementLock); }
    ~StartButtonLock() { ReleaseSRWLockExclusive(&g_startButtonElementLock); }
};
static void RememberStartButton(const winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton& element) {
    try {
        // Capture the dispatcher here, on the thread that owns the element.
        // Looking it up during launcher teardown can block on that thread.
        StartButtonReference next{winrt::make_weak(element), element.Dispatcher()};
        StartButtonLock lock;
        std::swap(next, g_startButtonReference);
    } catch (...) {}
}
static StartButtonReference StartButtonReferenceForReset() {
    StartButtonLock lock;
    return g_startButtonReference;
}
void ForgetStartButton() {
    StartButtonReference previous;
    StartButtonLock lock;
    std::swap(previous, g_startButtonReference);
}
// A queued reset is a lambda in this image, so it may not outlive the module.
// The counter, the gate and the drain live in launchpad_lifecycle.h; the gate
// is closed in Wh_ModBeforeUninit and Wh_ModUninit waits for the count.
// The body runs on the thread that owns the element.
static void ClearStartButtonLatchNow(const winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton& element,const wchar_t* why) {
    try {
        auto checked=element.IsChecked();
        // A null checked state is the three-state "indeterminate" —
        // it reads as neither true nor false but still paints a
        // pressed-looking capsule, so it counts as a latch too.
        bool latch=!checked||checked.Value();
        if(latch)element.IsChecked(winrt::box_value(false).as<winrt::Windows::Foundation::IReference<bool>>());
        // The keyboard path also leaves the keyboard focus ring on the
        // button: Start would hand it focus and move it away on close,
        // and neither happens when the menu is replaced.
        int focus=-2;
        auto control=element.try_as<winrt::Windows::UI::Xaml::Controls::Control>();
        if(control) {
            focus=(int)control.FocusState();
            if(control.FocusState()!=winrt::Windows::UI::Xaml::FocusState::Unfocused)
                control.Focus(winrt::Windows::UI::Xaml::FocusState::Unfocused);
            // The capsule is painted by the template's visual states,
            // not by the logical properties above; a replaced toggle
            // can leave the state machine stuck mid-gesture. Force the
            // template back to its resting states as well.
            winrt::Windows::UI::Xaml::VisualStateManager::GoToState(control,L"Normal",false);
            winrt::Windows::UI::Xaml::VisualStateManager::GoToState(control,L"Unfocused",false);
        }
        StartStateTrace(why,latch?0:-1,element.IsPressed(),element.IsPointerOver(),focus);
    } catch(...) {}
}
void ClearStartButtonLatch(const wchar_t* why) {
    // The call itself is one unit of work: it runs module code right now, on a
    // hook thread that can still be inside this function when the unload
    // starts. Counting it is what stops the drain from seeing zero while this
    // thread has not even reached the queue yet.
    StartLatchTicket invocation;
    if (g_startLatchStopping.load() || g_nativeStartPassthrough.load()) return;
    auto reference = StartButtonReferenceForReset();
    if (!reference.dispatcher) return;
    g_startLatchPending.fetch_add(1);
    if (g_startLatchStopping.load()) {
        FinishStartLatchReset();
        return;
    }
    try {
        reference.dispatcher.RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal,
            [weak = std::move(reference.element), why]() {
                struct Release { ~Release() { FinishStartLatchReset(); } } release;
                // A reset queued before Ctrl+Esc may run after native Start
                // takes ownership. Leave its checked and focus states alone.
                if (g_nativeStartPassthrough.load()) return;
                try {
                    if (auto element = weak.get()) ClearStartButtonLatchNow(element, why);
                } catch (...) {}
            });
    } catch (...) {
        FinishStartLatchReset();
    }
}
winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton StartToggleElement(void* object) {
    try {
        winrt::Windows::Foundation::IInspectable value{nullptr};winrt::copy_from_abi(value,object);
        auto element=value.try_as<winrt::Windows::UI::Xaml::FrameworkElement>();
        if(element&&winrt::Windows::UI::Xaml::Automation::AutomationProperties::GetAutomationId(element)==L"StartButton")
            return element.try_as<winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton>();
    } catch(...) {}
    return nullptr;
}
using XamlStartToggle_t = HRESULT(WINAPI*)(void*);
XamlStartToggle_t g_xamlStartToggleOriginal = nullptr;
HRESULT WINAPI XamlStartToggleHook(void* object) {
    if (g_nativeStartPassthrough.load() || !g_wnd || g_startButtonAction == KeyAction::Start)
        return g_xamlStartToggleOriginal(object);

    auto element = StartToggleElement(object);
    if (!element) return g_xamlStartToggleOriginal(object);
    RememberStartButton(element);
    try {
        element.IsChecked(winrt::box_value(false).as<winrt::Windows::Foundation::IReference<bool>>());
        return S_OK;
    } catch (...) {
        return g_xamlStartToggleOriginal(object);
    }
}

using XamlStartVisible_t=HRESULT(WINAPI*)(void*,bool);
XamlStartVisible_t g_xamlStartVisibleOriginal=nullptr;
std::atomic<bool> g_xamlStartInstalling{false};
// Accessed only while holding the installation guard.
HMODULE g_xamlAttemptedModule = nullptr;
using XamlLoadLibrary_t=HMODULE(WINAPI*)(LPCWSTR,HANDLE,DWORD);
XamlLoadLibrary_t g_xamlLoadLibraryOriginal=nullptr;
bool HookXamlStartVisibility();
HMODULE WINAPI XamlTaskbarLoadHook(LPCWSTR name,HANDLE file,DWORD flags) {
    auto module=g_xamlLoadLibraryOriginal(name,file,flags);
    if(module&&
       (module==GetModuleHandleW(L"Taskbar.View.dll")||module==GetModuleHandleW(L"ExplorerExtensions.dll"))) {
        if(HookXamlStartVisibility())Wh_ApplyHookOperations();
    }
    return module;
}
HRESULT WINAPI XamlStartVisibleHook(void* object, bool visible) {
    // The shell can publish several visibility and checked changes as native
    // Start loses focus. Pass through the whole sequence, including nested
    // calls, until ShowStartView chooses a replacement again.
    if (!visible || g_nativeStartPassthrough.load() || !g_wnd ||
        (g_startButtonAction == KeyAction::Start && !g_startButtonReplacementActive)) {
        return g_xamlStartVisibleOriginal(object, visible);
    }

    auto element = StartToggleElement(object);
    if (!element) return g_xamlStartVisibleOriginal(object, visible);
    RememberStartButton(element);
    ClearStartButtonLatch(L"latch-visible-clear");
    return g_xamlStartVisibleOriginal(object, false);
}

bool HookXamlStartVisibility() {
    if (g_xamlStartInstalling.exchange(true)) return false;
    struct InstallingGuard {
        ~InstallingGuard() { g_xamlStartInstalling = false; }
    } guard;
    if (g_xamlStartVisibleOriginal) return false;
    auto module=GetModuleHandleW(L"Taskbar.View.dll");
    if(!module)module=GetModuleHandleW(L"ExplorerExtensions.dll");
    if(!module){
        // Hook the kernelbase export, not the kernel32 thunk: a call that goes
        // straight to kernelbase (WinRT activation of Taskbar.View.dll) never
        // reaches a hook placed on the kernel32 import.
        auto kernelbase=GetModuleHandleW(L"kernelbase.dll");
        auto loadLibrary=kernelbase
            ?reinterpret_cast<XamlLoadLibrary_t>(GetProcAddress(kernelbase,"LoadLibraryExW"))
            :nullptr;
        if(loadLibrary&&!g_xamlLoadLibraryOriginal){
            // Registered here; Windhawk applies queued hooks when Wh_ModInit
            // returns, so no explicit apply is needed (and during init the API
            // refuses one anyway).
            WindhawkUtils::SetFunctionHook(loadLibrary,XamlTaskbarLoadHook,
                                           &g_xamlLoadLibraryOriginal);
        }
        Wh_Log(L"XAML taskbar not loaded; waiting for its module load.");return false;
    }
    // HookSymbols installs whatever it resolves before it reports failure, so a
    // second attempt on the same image would queue the same hooks again. One
    // attempt per loaded module is enough; a different base address means the
    // module was reloaded and the hooks really are gone.
    if (g_xamlAttemptedModule == module) return false;
    g_xamlAttemptedModule = module;
    // Taskbar.View.dll, ExplorerExtensions.dll
    WindhawkUtils::SYMBOL_HOOK taskbarViewHooks[]={
        {{LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::ExperienceToggleButton,struct winrt::Windows::UI::Xaml::Controls::Primitives::IToggleButtonOverrides>::OnToggle(void))"},
            &g_xamlStartToggleOriginal,XamlStartToggleHook,true},
        {{LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::ExperienceToggleButton,struct winrt::Taskbar::IExperienceToggleButton>::put_IsExperienceVisible(bool))"},
            &g_xamlStartVisibleOriginal,XamlStartVisibleHook,true},
    };
    const bool hadToggle = g_xamlStartToggleOriginal != nullptr;
    const auto started = GetTickCount64();
    Wh_Log(L"Resolving Start button symbols");
    const bool resolved = WindhawkUtils::HookSymbols(module, taskbarViewHooks, ARRAYSIZE(taskbarViewHooks));
    const bool hasToggle = g_xamlStartToggleOriginal != nullptr;
    const bool hasVisibility = g_xamlStartVisibleOriginal != nullptr;
    // Keep the hook surface on WinRT ABI interfaces. The private checked-change
    // callback takes references to projected C++ objects, which have a different ABI.
    Wh_Log(L"Start button symbols resolved=%d toggle=%d visibility=%d elapsed=%llu ms",
           resolved, hasToggle, hasVisibility, GetTickCount64() - started);
    // A resolved optional hook can be queued even if its companion is missing.
    return (hasToggle && !hadToggle) || hasVisibility;
}

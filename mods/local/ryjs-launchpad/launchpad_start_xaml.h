
// Windows 11 Start is a XAML ExperienceToggleButton, not a Win32 "Start"
// child window. Its OnToggle latches IsChecked before Click reaches our
// ShowStartView hook. Intercept that toggle when replacing the native menu;
// otherwise no native menu-close notification ever clears the checked state.
// Hook the WinRT ABI setter: pThis is an actual COM interface, so identifying
// the element requires no implementation offsets or foreign memory writes.
std::atomic<bool> g_nativeStartPassthrough{false};
void StartStateTrace(PCWSTR event,int checked=-2,int pressed=-2,int hover=-2,int focus=-2) {
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
// The keyboard path latches the button's checked state without ever running
// OnToggle (no trace, no hook there), and the native menu close that would
// clear it never happens either. Remember the element whenever a hook sees it,
// so the launcher can wipe the latch when it opens and hides. XAML objects
// live on the taskbar's UI thread, so the reset is posted through the
// element's own dispatcher.
static winrt::weak_ref<winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton> g_startButtonElement;
static void RememberStartButton(const winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton& element) {
    try {g_startButtonElement=winrt::make_weak(element);} catch(...) {}
}
void ClearStartButtonLatch(const wchar_t* why) {
    auto element=g_startButtonElement.get();
    if(!element) {
        // No hook has seen the button yet: the clear could not run. Trace it,
        // so a silent no-op is never mistaken for a working reset.
        StartStateTrace(why,-3,-3,-3);
        return;
    }
    try {
        auto dispatcher=element.Dispatcher();
        dispatcher.RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal,[element,why]() {
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
        });
    } catch(...) {}
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
void TraceStartElement(PCWSTR event,winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton const& element) {
    if(!element)return;
    try {auto checked=element.IsChecked();StartStateTrace(event,checked?(checked.Value()?1:0):-1,element.IsPressed(),element.IsPointerOver());}catch(...){}
}
using XamlStartToggle_t=HRESULT(WINAPI*)(void*);
using XamlStartPointer_t=HRESULT(WINAPI*)(void*,void*);
XamlStartToggle_t g_xamlStartToggleOriginal=nullptr;
XamlStartPointer_t g_xamlStartPressedOriginal=nullptr,g_xamlStartReleasedOriginal=nullptr,g_xamlStartExitedOriginal=nullptr;
HRESULT WINAPI XamlStartToggleHook(void* object) {
    auto element=StartToggleElement(object);
    if(element)RememberStartButton(element);
    TraceStartElement(L"toggle-before",element);
    if(element&&g_wnd&&g_startButtonAction.load()!=KeyAction::Start) {
        // Only the selection latch belongs to the native menu. Leave the
        // pointer lifecycle and the following Click/ShowStartView untouched.
        // Also clear a latch left by a previous version or native-menu click.
        try {element.IsChecked(winrt::box_value(false).as<winrt::Windows::Foundation::IReference<bool>>());}
        catch(...) {return g_xamlStartToggleOriginal(object);}
        TraceStartElement(L"toggle-replaced",element);
        return S_OK;
    }
    auto result=g_xamlStartToggleOriginal(object);
    TraceStartElement(L"toggle-after",element);return result;
}
HRESULT WINAPI XamlStartPressedHook(void* object,void* args) {
    auto element=StartToggleElement(object);TraceStartElement(L"press-before",element);
    auto result=g_xamlStartPressedOriginal(object,args);TraceStartElement(L"press-after",element);return result;
}
HRESULT WINAPI XamlStartReleasedHook(void* object,void* args) {
    auto element=StartToggleElement(object);TraceStartElement(L"release-before",element);
    auto result=g_xamlStartReleasedOriginal(object,args);TraceStartElement(L"release-after",element);return result;
}
HRESULT WINAPI XamlStartExitedHook(void* object,void* args) {
    auto element=StartToggleElement(object);
    auto result=g_xamlStartExitedOriginal(object,args);
    TraceStartElement(L"exit-after",element);return result;
}
// IsChecked's dependency-property callback: every change of the checked state
// passes here, including the programmatic writes of the taskbar's view model.
// The keyboard path latches the button through exactly such a write — it
// never runs OnToggle and never touches IsExperienceVisible, so without this
// hook both the latch and the element reference behind ClearStartButtonLatch
// stay invisible. The sender is the button itself (a DependencyObject).
using XamlStartCheckedChange_t=void(WINAPI*)(void*,void*,void*);
XamlStartCheckedChange_t g_xamlStartCheckedChangeOriginal=nullptr;
void WINAPI XamlStartCheckedChangeHook(void* pThis,void* sender,void* property) {
    auto element=sender?StartToggleElement(sender):nullptr;
    if(element)RememberStartButton(element);
    g_xamlStartCheckedChangeOriginal(pThis,sender,property);
    TraceStartElement(L"checked-change",element);
    bool replacing=g_wnd&&!g_nativeStartPassthrough&&
        (g_startButtonAction!=KeyAction::Start||g_startButtonReplacementActive);
    if(element&&replacing)ClearStartButtonLatch(L"latch-checked-clear");
}
using XamlStartVisible_t=HRESULT(WINAPI*)(void*,bool);
XamlStartVisible_t g_xamlStartVisibleOriginal=nullptr;
std::atomic<bool> g_xamlStartInstalling{false};
using XamlLoadLibrary_t=HMODULE(WINAPI*)(LPCWSTR,HANDLE,DWORD);
XamlLoadLibrary_t g_xamlLoadLibraryOriginal=nullptr;
bool HookXamlStartVisibility();
HMODULE WINAPI XamlTaskbarLoadHook(LPCWSTR name,HANDLE file,DWORD flags) {
    auto module=g_xamlLoadLibraryOriginal(name,file,flags);
    if(module&&!g_xamlStartVisibleOriginal&&
       (module==GetModuleHandleW(L"Taskbar.View.dll")||module==GetModuleHandleW(L"ExplorerExtensions.dll"))) {
        if(HookXamlStartVisibility())Wh_ApplyHookOperations();
    }
    return module;
}
HRESULT WINAPI XamlStartVisibleHook(void* object,bool visible) {
    bool start=false;
    winrt::Windows::Foundation::IInspectable inspectable{nullptr};
    try {
        winrt::copy_from_abi(inspectable,object);
        auto element=inspectable.try_as<winrt::Windows::UI::Xaml::FrameworkElement>();
        start=element&&winrt::Windows::UI::Xaml::Automation::AutomationProperties::GetAutomationId(element)==L"StartButton";
    } catch(...) { }
    if(start) {
        bool replacing=g_wnd&&!g_nativeStartPassthrough&&
            (g_startButtonAction!=KeyAction::Start||g_startButtonReplacementActive);
        bool effective=visible&&!replacing;
        // The keyboard path never reaches the toggle or pointer hooks, so the
        // visibility request is the only trace it leaves. Log the element's
        // state with it: this is where a keyboard-driven latch becomes
        // visible.
        auto toggle=start?inspectable.try_as<winrt::Windows::UI::Xaml::Controls::Primitives::ToggleButton>():nullptr;
        if(toggle)RememberStartButton(toggle);
        try {
            if(toggle) {
                auto checked=toggle.IsChecked();
                StartStateTrace(visible?L"visible-true":L"visible-false",
                    checked?(checked.Value()?1:0):-1,toggle.IsPressed(),toggle.IsPointerOver());
            } else StartStateTrace(visible?L"visible-true":L"visible-false");
        } catch(...) {StartStateTrace(visible?L"visible-true-err":L"visible-false-err");}
        Wh_Log(L"XAML Start visibility requested=%d effective=%d replacing=%d native=%d",
            visible,effective,replacing,(bool)g_nativeStartPassthrough);
        if(visible&&replacing)ClearStartButtonLatch(L"latch-visible-clear");
        auto result=g_xamlStartVisibleOriginal(object,effective);
        if(!visible)g_nativeStartPassthrough=false;
        return result;
    }
    return g_xamlStartVisibleOriginal(object,visible);
}
bool HookXamlStartVisibility() {
    if(g_xamlStartVisibleOriginal)return true;
    if(g_xamlStartInstalling.exchange(true))return false;
    struct InstallingGuard {~InstallingGuard(){g_xamlStartInstalling=false;}} guard;
    auto module=GetModuleHandleW(L"Taskbar.View.dll");
    if(!module)module=GetModuleHandleW(L"ExplorerExtensions.dll");
    if(!module){
        if(!g_xamlLoadLibraryOriginal)Wh_SetFunctionHook((void*)LoadLibraryExW,(void*)XamlTaskbarLoadHook,(void**)&g_xamlLoadLibraryOriginal);
        Wh_Log(L"XAML taskbar not loaded; waiting for its module load.");return false;
    }
    // Taskbar.View.dll
    WindhawkUtils::SYMBOL_HOOK taskbarViewHooks[]={
        {{LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::ExperienceToggleButton,struct winrt::Windows::UI::Xaml::Controls::Primitives::IToggleButtonOverrides>::OnToggle(void))"},
            &g_xamlStartToggleOriginal,XamlStartToggleHook,true},
        {{LR"(private: void __cdecl winrt::Taskbar::implementation::ExperienceToggleButton::OnIsCheckedPropertyChanged(struct winrt::Windows::UI::Xaml::DependencyObject const &,struct winrt::Windows::UI::Xaml::DependencyProperty const &))"},
            &g_xamlStartCheckedChangeOriginal,XamlStartCheckedChangeHook,true},
        {{LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::ExperienceToggleButton,struct winrt::Windows::UI::Xaml::Controls::IControlOverrides>::OnPointerPressed(void *))"},
            &g_xamlStartPressedOriginal,XamlStartPressedHook,true},
        {{LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::ExperienceToggleButton,struct winrt::Windows::UI::Xaml::Controls::IControlOverrides>::OnPointerReleased(void *))"},
            &g_xamlStartReleasedOriginal,XamlStartReleasedHook,true},
        {{LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::ExperienceToggleButton,struct winrt::Windows::UI::Xaml::Controls::IControlOverrides>::OnPointerExited(void *))"},
            &g_xamlStartExitedOriginal,XamlStartExitedHook,true},
        {{LR"(public: virtual int __cdecl winrt::impl::produce<struct winrt::Taskbar::implementation::ExperienceToggleButton,struct winrt::Taskbar::IExperienceToggleButton>::put_IsExperienceVisible(bool))"},
            &g_xamlStartVisibleOriginal,XamlStartVisibleHook,true},
    };
    bool ok=WindhawkUtils::HookSymbols(module,taskbarViewHooks,ARRAYSIZE(taskbarViewHooks))&&g_xamlStartVisibleOriginal;
    // The checked-change hook is the one that sees the keyboard path's latch:
    // record whether its symbol actually resolved.
    StartStateTrace(g_xamlStartCheckedChangeOriginal?L"hooks-installed-cc1":L"hooks-installed-cc0",
        g_xamlStartToggleOriginal!=nullptr,g_xamlStartPressedOriginal!=nullptr,g_xamlStartReleasedOriginal!=nullptr);
    Wh_Log(L"XAML Start visibility hook available=%d",ok);return ok;
}

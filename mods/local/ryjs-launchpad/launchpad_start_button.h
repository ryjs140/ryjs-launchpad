#pragma once

// Classic/StartAllBack taskbars latch their Start button independently of
// XamlLauncher. Consume the complete mouse gesture before that native latch
// runs; ShowStartView remains responsible for keyboard and other callers.
constexpr WCHAR kStartPressProperty[] = L"Launchpad.StartPress." WH_MOD_ID;
std::vector<HWND> g_startButtonWindows;  // Launcher thread only.
HWINEVENTHOOK g_startButtonEvents;
std::atomic<bool> g_startButtonReplacementActive{false};

UINT StartButtonCleanupMessage() {
    static UINT message = RegisterWindowMessageW(L"Launchpad.StartButtonCleanup." WH_MOD_ID);
    return message;
}
UINT StartButtonResetMessage() {
    static UINT message = RegisterWindowMessageW(L"Launchpad.StartButtonReset." WH_MOD_ID);
    return message;
}

bool WindowClassIs(HWND window, PCWSTR expected) {
    WCHAR name[64];
    return GetClassNameW(window, name, ARRAYSIZE(name)) &&
           _wcsicmp(name, expected) == 0;
}

HWND TaskbarStartButton(HWND window) {
    DWORD pid = 0;
    GetWindowThreadProcessId(window, &pid);
    if (pid != GetCurrentProcessId()) return nullptr;
    HWND root = GetAncestor(window, GA_ROOT);
    if (!WindowClassIs(root, L"Shell_TrayWnd") &&
        !WindowClassIs(root, L"Shell_SecondaryTrayWnd") &&
        !WindowClassIs(root, L"SIBTaskbar")) return nullptr;
    for (HWND current = window; current && current != root; current = GetParent(current)) {
        if (WindowClassIs(current, L"Start") || WindowClassIs(current, L"SIBOrb"))
            return current;
    }
    return nullptr;
}

void ClearStartButtonPress(HWND window) {
    RemovePropW(window, kStartPressProperty);
    if (GetCapture() == window) ReleaseCapture();
}

LRESULT CALLBACK StartButtonSubclass(HWND window, UINT message, WPARAM wp,
                                    LPARAM lp, DWORD_PTR button) {
    if(message==WM_LBUTTONDOWN||message==WM_LBUTTONUP||message==WM_CAPTURECHANGED||
       message==WM_CANCELMODE||message==WM_MOUSELEAVE||message==WM_KILLFOCUS) {
        Wh_Log(L"Start lifecycle hwnd=%p button=%p msg=%04x press=%lld capture=%p foreground=%p",
            window,(HWND)button,message,(long long)(INT_PTR)GetPropW(window,kStartPressProperty),GetCapture(),GetForegroundWindow());
    }
    if (message == StartButtonResetMessage()) {
        // The mouse pair alone does not clear a previously latched state or
        // the native hover left behind when the launcher covers the button.
        // Perform cleanup on its owning thread, after the click has unwound.
        if ((wp && !g_startButtonReplacementActive) ||
            (!wp && g_startButtonAction == KeyAction::Start) || (INT_PTR)GetPropW(window,kStartPressProperty)>1) return 0;
        DefSubclassProc(window, WM_CANCELMODE, 0, 0);
        DefSubclassProc(window, BM_SETSTATE, FALSE, 0);
        DefSubclassProc(window, BM_SETCHECK, BST_UNCHECKED, 0);
        DefSubclassProc(window, WM_MOUSELEAVE, 0, 0);
        InvalidateRect(window, nullptr, TRUE);
        return 0;
    }
    if (message == StartButtonCleanupMessage() || message == WM_NCDESTROY) {
        ClearStartButtonPress(window);
        return DefSubclassProc(window, message, wp, lp);
    }
    switch (message) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_NCLBUTTONDOWN:
        case WM_NCLBUTTONDBLCLK: {
            KeyAction action = g_startButtonAction.load();
            if (!g_wnd || action == KeyAction::Start) break;
            // 1 means cancelled; other values remember the action at press
            // time, so a settings change cannot split the down/up pair.
            if (!SetPropW(window, kStartPressProperty,
                          (HANDLE)(INT_PTR)(2 + (int)action))) break;
            SetCapture(window);
            if (GetCapture() != window) {
                RemovePropW(window, kStartPressProperty);
                break;
            }
            return 0;
        }
        case WM_LBUTTONUP:
        case WM_NCLBUTTONUP: {
            INT_PTR press = (INT_PTR)GetPropW(window, kStartPressProperty);
            if (!press) break;
            bool captured = GetCapture() == window;
            ClearStartButtonPress(window);
            POINT point{(short)LOWORD(lp), (short)HIWORD(lp)};
            bool validPoint = message == WM_NCLBUTTONUP || ClientToScreen(window, &point);
            RECT rect;
            HWND launcher = g_wnd;
            g_startButtonReplacementActive = true;
            PostMessageW(window, StartButtonResetMessage(), 1, 0);
            if (press > 1 && captured && validPoint && launcher &&
                GetWindowRect((HWND)button, &rect) && PtInRect(&rect, point)) {
                KeyAction action = (KeyAction)(press - 2);
                PostMessageW(launcher, action == KeyAction::Program ? WM_APP_RUN : WM_APP_TOGGLE, 0, 0);
            }
            // Even a cancelled gesture owns its release. Never give the
            // native button an up for a down that it did not receive.
            return 0;
        }
        case WM_CAPTURECHANGED:
        case WM_CANCELMODE:
            if (GetPropW(window, kStartPressProperty)) {
                // Keep a release tombstone so an unmatched UP stays consumed,
                // but do not let that cancelled gesture block visual cleanup.
                SetPropW(window, kStartPressProperty, (HANDLE)1);
                if (message == WM_CANCELMODE && GetCapture() == window) ReleaseCapture();
                g_startButtonReplacementActive=true;
                PostMessageW(window,StartButtonResetMessage(),1,0);
            }
            break;
    }
    return DefSubclassProc(window, message, wp, lp);
}

void AttachStartButtonWindow(HWND window) {
    HWND button = TaskbarStartButton(window);
    if (!button) return;
    // Reapplying the same subclass is idempotent, including after HWND reuse.
    if (!WindhawkUtils::SetWindowSubclassFromAnyThread(window, StartButtonSubclass, (DWORD_PTR)button)) {
        Wh_Log(L"Couldn't subclass Start button: %u", GetLastError());
        return;
    }
    if (std::find(g_startButtonWindows.begin(), g_startButtonWindows.end(), window) == g_startButtonWindows.end()) {
        g_startButtonWindows.push_back(window);
        Wh_Log(L"Start button subclass attached: %p (button %p)", window, button);
    }
    PostMessageW(window, StartButtonResetMessage(), 0, 0);
}

void ResetTaskbarStartButtons(bool ownRequest) {
    if (ownRequest) g_startButtonReplacementActive = true;
    if (!g_startButtonReplacementActive) return;
    for (HWND window : g_startButtonWindows)
        PostMessageW(window, StartButtonResetMessage(), 1, 0);
}

BOOL CALLBACK AttachStartButtonChild(HWND window, LPARAM) {
    AttachStartButtonWindow(window);
    return TRUE;
}

BOOL CALLBACK AttachTaskbarStartButtons(HWND window, LPARAM) {
    DWORD pid = 0;
    GetWindowThreadProcessId(window, &pid);
    if (pid == GetCurrentProcessId() &&
        (WindowClassIs(window, L"Shell_TrayWnd") ||
         WindowClassIs(window, L"Shell_SecondaryTrayWnd") || WindowClassIs(window, L"SIBTaskbar")))
        EnumChildWindows(window, AttachStartButtonChild, 0);
    return TRUE;
}

void CALLBACK StartButtonWindowEvent(HWINEVENTHOOK, DWORD event, HWND window,
                                    LONG object, LONG child, DWORD, DWORD) {
    if (!g_startButtonEvents || !window || object != OBJID_WINDOW || child != CHILDID_SELF ||
        (event != EVENT_OBJECT_CREATE && event != EVENT_OBJECT_SHOW)) return;
    AttachStartButtonWindow(window);
    // A taskbar or orb can be announced after its children were created.
    if (TaskbarStartButton(window) || WindowClassIs(window, L"Shell_TrayWnd") ||
        WindowClassIs(window, L"Shell_SecondaryTrayWnd") || WindowClassIs(window, L"SIBTaskbar"))
        EnumChildWindows(window, AttachStartButtonChild, 0);
}

void StartButtonHooksStart() {
    // Register before scanning, so taskbars created during the scan are not
    // missed. Out-of-context callbacks run on this existing message loop.
    g_startButtonEvents = SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_SHOW, nullptr,
        StartButtonWindowEvent, GetCurrentProcessId(), 0, WINEVENT_OUTOFCONTEXT);
    if (!g_startButtonEvents) Wh_Log(L"Start button window events unavailable: %u", GetLastError());
    EnumWindows(AttachTaskbarStartButtons, 0);
}

void StartButtonHooksStop() {
    if (g_startButtonEvents) {
        UnhookWinEvent(g_startButtonEvents);
        g_startButtonEvents = nullptr;
    }
    for (HWND window : g_startButtonWindows) {
        DWORD pid = 0;
        GetWindowThreadProcessId(window, &pid);
        if (pid != GetCurrentProcessId()) continue;
        // Cleanup runs on the button's thread before detaching the callback.
        SendMessageW(window, StartButtonCleanupMessage(), 0, 0);
        WindhawkUtils::RemoveWindowSubclassFromAnyThread(window, StartButtonSubclass);
    }
    g_startButtonWindows.clear();
}

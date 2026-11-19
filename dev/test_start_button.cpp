// Isolated Win32 regression test: hidden fake taskbars, no Explorer injection
// or physical mouse/keyboard input. Exercises the production subclass itself.
#include <windows.h>
#include <commctrl.h>
#include <algorithm>
#include <atomic>
#include <cstdio>
#include <vector>
#define WH_MOD_ID L"ryjs-launchpad-test"
#define Wh_Log(...) ((void)0)
enum class KeyAction { Grid, Start, Program };
std::atomic<KeyAction> g_startButtonAction{KeyAction::Grid};
HWND g_wnd;
constexpr UINT WM_APP_TOGGLE = WM_APP + 1;
constexpr UINT WM_APP_RUN = WM_APP + 5;

namespace WindhawkUtils {
using Proc = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM, DWORD_PTR);
LRESULT CALLBACK Wrapper(HWND w, UINT m, WPARAM p, LPARAM l, UINT_PTR id, DWORD_PTR data) {
    if (m == WM_NCDESTROY) RemoveWindowSubclass(w, Wrapper, id);
    return ((Proc)id)(w, m, p, l, data);
}
BOOL SetWindowSubclassFromAnyThread(HWND w, Proc p, DWORD_PTR data) {
    // All fake windows belong to the test thread. Production uses Windhawk's
    // cross-thread marshalling implementation from windhawk_utils.h.
    return SetWindowSubclass(w, Wrapper, (UINT_PTR)p, data);
}
void RemoveWindowSubclassFromAnyThread(HWND w, Proc p) {
    RemoveWindowSubclass(w, Wrapper, (UINT_PTR)p);
}
}
#include "../mods/local/seamless-launchpad/launchpad_start_button.h"

int failures, checks, nativeDown, nativeUp, rightClicks, toggles, runs;
bool nativeChecked;
bool nativePushed, nativeHot;
void Check(bool condition, const char* what) {
    ++checks;
    if (!condition) { ++failures; printf("FAIL: %s\n", what); }
}
LRESULT CALLBACK FakeWindow(HWND w, UINT m, WPARAM p, LPARAM l) {
    if (m == BM_SETCHECK) { nativeChecked = p != 0; return 0; }
    if (m == BM_SETSTATE) { nativePushed = p != 0; return 0; }
    if (m == WM_MOUSELEAVE) { nativeHot = false; return 0; }
    if (m == WM_LBUTTONDOWN) { ++nativeDown; return 0; }
    if (m == WM_LBUTTONUP) { ++nativeUp; nativeChecked = !nativeChecked; return 0; }
    if (m == WM_RBUTTONUP) { ++rightClicks; return 0; }
    if (m == WM_APP_TOGGLE) { ++toggles; return 0; }
    if (m == WM_APP_RUN) { ++runs; return 0; }
    return DefWindowProcW(w, m, p, l);
}
void Pump() {
    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg); DispatchMessageW(&msg);
    }
}
void Click(HWND w, LPARAM point = MAKELPARAM(10, 10)) {
    SendMessageW(w, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(10, 10));
    SendMessageW(w, WM_LBUTTONUP, 0, point);
    Pump();
}
HWND Create(PCWSTR cls, HWND parent = nullptr) {
    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = FakeWindow;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = cls;
    RegisterClassExW(&wc);
    HWND w = CreateWindowExW(0, cls, L"isolated test", parent ? WS_CHILD : WS_POPUP,
                             0, 0, 100, 100, parent, nullptr, wc.hInstance, nullptr);
    Check(w != nullptr, "create hidden fixture window");
    return w;
}
int main() {
    g_wnd = Create(L"LaunchpadTestReceiver");
    HWND tray = Create(L"Shell_TrayWnd");
    HWND button = Create(L"Start", tray);
    Click(button);
    Check(nativeChecked, "baseline reproduces native latch without router");
    nativeChecked = false; nativeDown = nativeUp = 0;

    HWND unrelated = Create(L"Start");
    Check(TaskbarStartButton(unrelated) == nullptr, "ignore Start class outside taskbar");
    nativeChecked=nativePushed=nativeHot=true;
    StartButtonHooksStart();
    Pump();
    Check(!nativeChecked && !nativePushed && !nativeHot, "initial attachment clears pre-existing checked/pushed/hover state");
    Check(g_startButtonEvents != nullptr, "window event registration");
    Check(std::find(g_startButtonWindows.begin(), g_startButtonWindows.end(), button) != g_startButtonWindows.end(),
          "initial taskbar scan attaches button");
    Click(button); Click(button);
    Check(toggles == 2 && !nativeChecked && nativeDown == 0 && nativeUp == 0,
          "repeated clicks toggle once each without native latch");
    Check(!GetPropW(button, kStartPressProperty) && GetCapture() != button, "release clears capture and press");
    nativeChecked=nativePushed=nativeHot=true;
    ResetTaskbarStartButtons(true); Pump();
    Check(!nativeChecked && !nativePushed && !nativeHot, "launcher show/close reset clears native visual states");
    g_startButtonReplacementActive=false; nativeChecked=nativePushed=nativeHot=true;
    PostMessageW(button,StartButtonResetMessage(),1,0); Pump();
    Check(nativeChecked && nativePushed && nativeHot, "queued reset does not clear a later original Start action");
    ResetTaskbarStartButtons(true); Pump();

    Click(button, MAKELPARAM(200, 200));
    Check(toggles == 2 && nativeUp == 0, "release outside cancels and consumes up");
    SendMessageW(button, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(10, 10));
    SetCapture(unrelated);
    SendMessageW(button, WM_LBUTTONUP, 0, MAKELPARAM(10, 10));
    ReleaseCapture(); Pump();
    Check(toggles == 2 && nativeUp == 0, "lost capture cancels complete gesture");
    SendMessageW(button, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(10, 10));
    SendMessageW(button, WM_CANCELMODE, 0, 0);
    nativeChecked=nativePushed=nativeHot=true;
    Pump();
    Check(!nativeChecked&&!nativePushed&&!nativeHot,
        "cancel without a later UP releases visual latch (cancel tombstone cannot block reset)");
    SendMessageW(button, WM_LBUTTONUP, 0, MAKELPARAM(10, 10)); Pump();
    Check(toggles == 2 && GetCapture() != button && nativeUp == 0, "cancel mode consumes pending up");

    SendMessageW(button, WM_RBUTTONUP, 0, MAKELPARAM(10, 10));
    Check(rightClicks == 1, "right-click reaches native context menu path");
    g_startButtonAction = KeyAction::Start;
    Click(button);
    Check(nativeDown == 1 && nativeUp == 1 && nativeChecked && toggles == 2,
          "original Start action receives native down/up");
    nativeChecked = false;
    g_startButtonAction = KeyAction::Program;
    Click(button);
    Check(runs == 1 && nativeUp == 1, "program action uses existing run message");
    SendMessageW(button, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(10, 10));
    g_startButtonAction = KeyAction::Start;
    SendMessageW(button, WM_LBUTTONUP, 0, MAKELPARAM(10, 10)); Pump();
    Check(runs == 2 && nativeUp == 1, "settings change preserves captured action");
    g_startButtonAction = KeyAction::Grid;

    HWND secondary = Create(L"Shell_SecondaryTrayWnd");
    HWND orb = Create(L"SIBOrb", secondary);
    HWND child = Create(L"LaunchpadTestOrbChild", orb);
    // Explicit notification also makes this deterministic on headless desktops
    // that do not broadcast accessibility creation events for hidden windows.
    StartButtonWindowEvent(g_startButtonEvents, EVENT_OBJECT_CREATE, orb, OBJID_WINDOW, CHILDID_SELF, 0, 0);
    Click(child);
    Check(toggles == 3 && nativeUp == 1, "new secondary orb and child are attached");
    DestroyWindow(orb);
    orb = Create(L"SIBOrb", secondary);
    StartButtonWindowEvent(g_startButtonEvents, EVENT_OBJECT_SHOW, orb, OBJID_WINDOW, CHILDID_SELF, 0, 0);
    Click(orb);
    Check(toggles == 4 && nativeUp == 1, "recreated button attaches without duplicate actions");

    SendMessageW(button, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(10, 10));
    StartButtonHooksStop();
    Check(GetCapture() != button && !GetPropW(button, kStartPressProperty) &&
          !g_startButtonEvents && g_startButtonWindows.empty(), "stop releases capture and detaches hooks");
    Click(button);
    Check(nativeDown == 2 && nativeUp == 2 && toggles == 4, "native button restored after stop");
    DestroyWindow(secondary); DestroyWindow(button); DestroyWindow(tray);
    DestroyWindow(unrelated); DestroyWindow(g_wnd);
    printf("%s: %d checks, %d failures\n", failures ? "FAIL" : "PASS", checks, failures);
    return failures ? 1 : 0;
}

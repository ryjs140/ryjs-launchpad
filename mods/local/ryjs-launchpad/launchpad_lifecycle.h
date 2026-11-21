#pragma once
// Lifecycle bookkeeping for work that runs on another thread's message loop.
// A queued lambda is code in this image: if the mod is unloaded while the
// taskbar's dispatcher still holds it, that thread runs unmapped code and
// Explorer crashes. The counter makes the wait possible, the gate stops new
// work once unload begins, and the drain runs after every hook is gone.
//
// Ownership rule: one count per unit of work that runs code in this image.
// A call to ClearStartButtonLatch is one unit (it runs now), and the lambda it
// queues is a second unit (it runs later). Each unit increments once and
// releases exactly once, so the counter can neither underflow nor reach zero
// while module code is still running or still queued.
//
// Separate from the XAML header so the test harness can exercise the drain
// without the WinRT hooks.

// Units of work that have not finished yet.
std::atomic<int> g_startLatchPending{0};
// Closed in Wh_ModBeforeUninit, while the hooks are still installed.
std::atomic<bool> g_startLatchStopping{false};
// Auto-reset, set when the count reaches zero. Created in Wh_ModInit.
HANDLE g_startLatchIdle = nullptr;

void FinishStartLatchReset() {
    int before = g_startLatchPending.fetch_sub(1);
    if (before <= 0) {
        // The protocol above never does this: every increment has exactly one
        // release. Keep a future break from turning into a drain that can
        // never finish, and leave a trace of it.
        g_startLatchPending.store(0);
        Wh_Log(L"Start-latch count released without a pending unit (%d); reset to zero", before);
        if (g_startLatchIdle) SetEvent(g_startLatchIdle);  // Let a waiting drain re-check.
        return;
    }
    if (before == 1 && g_startLatchIdle) SetEvent(g_startLatchIdle);
}

// One unit of work: counted on construction, released on destruction, whether
// the function returns normally or unwinds.
struct StartLatchTicket {
    StartLatchTicket() { g_startLatchPending.fetch_add(1); }
    ~StartLatchTicket() { FinishStartLatchReset(); }
    StartLatchTicket(const StartLatchTicket&) = delete;
    StartLatchTicket& operator=(const StartLatchTicket&) = delete;
};

// Called once from Wh_ModUninit. By then Windhawk has removed the hooks and
// the launcher thread is joined, so no new unit can be created: the count can
// only fall. A dispatcher that accepted a reset but never runs it means the
// taskbar thread is gone or blocked for longer than the timeout; the caller
// then keeps this image mapped rather than letting a late callback run
// unmapped code.
bool WaitForStartLatchResets(DWORD timeoutMs) {
    if (g_startLatchPending.load() == 0) return true;
    HANDLE event = g_startLatchIdle;
    if (!event) return false;
    ULONGLONG deadline = GetTickCount64() + timeoutMs;
    for (;;) {
        ULONGLONG now = GetTickCount64();
        if (now >= deadline) break;
        if (WaitForSingleObject(event, (DWORD)(deadline - now)) == WAIT_OBJECT_0 &&
            g_startLatchPending.load() == 0)
            return true;
    }
    return g_startLatchPending.load() == 0;
}

void PinModuleForStartLatch() {
    HMODULE module = nullptr;
    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
                           (PCWSTR)&PinModuleForStartLatch, &module)) {
        Wh_Log(L"A start-button reset is still queued at unload; the module stays mapped so the callback cannot run unmapped code");
    } else {
        // Nothing else can keep the callback's code mapped. Say so loudly: the
        // unload is about to free an image that a taskbar thread may still run.
        Wh_Log(L"Could not pin the module for a queued start-button reset: %u", GetLastError());
    }
}

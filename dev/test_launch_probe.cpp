#include <windows.h>
#include <shellapi.h>

// ShellExecute runs this as a GUI-subsystem process with no window.
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    int count = 0;
    auto args = CommandLineToArgvW(GetCommandLineW(), &count);
    if (!args) return 1;
    HANDLE marker = count == 2 ? CreateFileW(args[1], GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr)
        : INVALID_HANDLE_VALUE;
    LocalFree(args);
    if (marker == INVALID_HANDLE_VALUE) return 1;
    CloseHandle(marker);
    return 0;
}

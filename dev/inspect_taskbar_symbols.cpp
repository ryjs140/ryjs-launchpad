// Offline symbol audit only; never loads code into Explorer or changes a UI.
#include <windows.h>
#include <dbghelp.h>
#include <cstdio>
#include <cstring>
BOOL CALLBACK Found(PSYMBOL_INFO s,ULONG,void*) {
    if(strstr(s->Name,"ExperienceToggleButton")||strstr(s->Name,"LaunchListButton")){char name[16384]{};UnDecorateSymbolName(s->Name,name,sizeof(name),UNDNAME_COMPLETE);printf("%llx %s\n",s->Address,name);}
    return TRUE;
}
int main() {
    auto process=GetCurrentProcess();SymSetOptions(SYMOPT_DEFERRED_LOADS);
    if(!SymInitialize(process,"C:\\ProgramData\\Windhawk\\Engine\\Symbols",FALSE))return 1;
    auto base=SymLoadModuleExW(process,nullptr,L"C:\\Windows\\SystemApps\\MicrosoftWindows.Client.Core_cw5n1h2txyewy\\Taskbar.View.dll",nullptr,0,0,nullptr,0);
    if(!base){printf("load error %lu\n",GetLastError());return 2;}
    bool ok=SymEnumSymbols(process,base,nullptr,Found,nullptr);SymCleanup(process);return ok?0:3;
}

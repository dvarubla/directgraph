#include "DPIHelper.h"
#include <windows.h>
#include "util.h"

namespace directgraph {
    static volatile LONG canSetAware = 1;

    typedef enum _PROCESS_DPI_AWARENESS {
        PROCESS_DPI_UNAWARE            = 0,
        PROCESS_SYSTEM_DPI_AWARE       = 1,
        PROCESS_PER_MONITOR_DPI_AWARE  = 2
    } PROCESS_DPI_AWARENESS;

    void setDPIAware(){
        if(InterlockedExchange(&canSetAware, 0)) {
            bool status = false;
            HMODULE shcore = LoadLibraryW(L"shcore.dll");
            if(shcore != NULL){
                HRESULT (WINAPI *SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS) =
                    reinterpret_cast<HRESULT (WINAPI *)(PROCESS_DPI_AWARENESS)>
                    (GetProcAddress(shcore, "SetProcessDpiAwareness"));
                if (SetProcessDpiAwareness != NULL) {
                    SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
                    status = true;
                }
                FreeLibrary(shcore);
            }
            if(!status){
                HMODULE user32 = LoadLibraryW(L"user32.dll");
                if(user32 != NULL){
                    BOOL (WINAPI *SetProcessDPIAware)() =
                        reinterpret_cast<BOOL (WINAPI *)()>
                        (GetProcAddress(user32, "SetProcessDPIAware"));
                    if (SetProcessDPIAware != NULL) {
                        SetProcessDPIAware();
                    }
                    FreeLibrary(user32);
                }
            }
        }
    }
}

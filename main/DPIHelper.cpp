#include "DPIHelper.h"
#include <windows.h>
#include <iostream>
#include "util.h"

namespace directgraph {

    DPIHelper::DPIHelper(float dpiX, float dpiY) : _dpiX(dpiX), _dpiY(dpiY) {
    }

    const float DPIHelper::DEFAULT_DPIX = 96.0f;
    const float DPIHelper::DEFAULT_DPIY = 96.0f;

    int_fast32_t DPIHelper::toPixelsX(float dip) {
        return static_cast<int_fast32_t>(round(dip * _dpiX / DEFAULT_DPIX));
    }

    int_fast32_t DPIHelper::toPixelsY(float dip) {
        return static_cast<int_fast32_t>(round(dip * _dpiY / DEFAULT_DPIY));
    }

    DPIHelper::DPIHelper() {
    }

    void DPIHelper::setDpiX(float dpiX) {
        _dpiX = dpiX;
    }

    void DPIHelper::setDpiY(float dpiY) {
        _dpiY = dpiY;
    }

    DPIHelper::~DPIHelper() {
    }

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

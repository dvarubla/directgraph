#include "DX9Common.h"
#include "DX9Exception.h"
#include <windows.h>
#include <iostream>

namespace directgraph {
    DX9Common::DX9Common() : _d3d(NULL), _device(NULL) {
        _d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (_d3d == NULL) {
            THROW_EXC_CODE(
                    DX9Exception, CANT_CREATE_DIRECTX9, std::wstring(L"Can't create DirectX object")
            );
        }
        InitializeCriticalSection(&_mainCS);
    }

    void DX9Common::getDpi(float &dpiX, float &dpiY) {
        HDC dc = GetDC(NULL);
        dpiX = (float) GetDeviceCaps(dc, LOGPIXELSX);
        dpiY = (float) GetDeviceCaps(dc, LOGPIXELSY);
    }

    IDirect3DSwapChain9 *DX9Common::createSwapChain(HWND win, uint_fast32_t width, uint_fast32_t height) {
        IDirect3DSwapChain9 *swapChain = NULL;
        if (_device == NULL) {
            createDevice(win, width, height);
            _device->GetSwapChain(0, &swapChain);
        } else {
            _d3dpp.BackBufferWidth = width;
            _d3dpp.BackBufferHeight = height;
            _d3dpp.hDeviceWindow = win;
            if(_device->CreateAdditionalSwapChain(&_d3dpp, &swapChain) != D3D_OK){
                THROW_EXC_CODE(
                        DX9Exception, DX9_CANT_CREATE_EXTRA_WIN, std::wstring(L"Can't create extra window")
                );
            }
        }
        return swapChain;
    }

    void DX9Common::createDevice(HWND win, uint_fast32_t width, uint_fast32_t height) {
        D3DDEVTYPE devType;
        DWORD flags;
        checkCaps(devType, flags);
        D3DDISPLAYMODE displayMode = {};
        if(_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode) != D3D_OK){
            THROW_EXC_CODE(
                    DX9Exception, DX9_CANT_GET_DISPLAY_MODE, std::wstring(L"Can't get display mode")
            );
        }
        _format = displayMode.Format;
        ZeroMemory(&_d3dpp, sizeof(_d3dpp));
        _d3dpp.BackBufferFormat = _format;
        _d3dpp.Windowed = TRUE;
        _d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        _d3dpp.BackBufferWidth = width;
        _d3dpp.BackBufferHeight = height;

        if(_d3d->CreateDevice(D3DADAPTER_DEFAULT,
                           devType,
                           win,
                           flags,
                           &_d3dpp,
                           &_device
        ) != D3D_OK){
            THROW_EXC_CODE(
                    DX9Exception, DX9_CANT_CREATE_DEVICE, std::wstring(L"Can't create DirectX device")
            );
        }
        try {
            if (_device->SetRenderState(D3DRS_LIGHTING, FALSE) != D3D_OK) {
                THROW_EXC_CODE(
                        DX9Exception, DX9_CANT_SET_RENDER_STATE, std::wstring(L"Can't disable lighting")
                );
            }
            if (_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE) != D3D_OK) {
                THROW_EXC_CODE(
                        DX9Exception, DX9_CANT_SET_RENDER_STATE, std::wstring(L"Can't disable cull mode")
                );
            }
            if (_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE) != D3D_OK) {
                THROW_EXC_CODE(
                        DX9Exception, DX9_CANT_SET_RENDER_STATE, std::wstring(L"Can't disable z-buffer")
                );
            }
        } catch(const std::exception &){
            _device->Release();
            throw;
        }
    }

    void DX9Common::checkCaps(D3DDEVTYPE &devType, DWORD &flags) {
        D3DCAPS9 caps = {};
        HRESULT hRes = _d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
        if (FAILED(hRes)) {
            devType = D3DDEVTYPE_REF;
            flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        } else {
            if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
                flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
            } else {
                flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            }
            devType = D3DDEVTYPE_HAL;
        }
    }

    void DX9Common::deleteSwapChain(IDirect3DSwapChain9 *swapChain) {
        if(swapChain != NULL) {
            swapChain->Release();
        }
    }

    D3DFORMAT DX9Common::getFormat() {
        return _format;
    }

    void DX9Common::lock() {
        EnterCriticalSection(&_mainCS);
    }

    void DX9Common::unlock() {
        LeaveCriticalSection(&_mainCS);
    }

    DX9Common::~DX9Common() {
        _device->Release();
        _d3d->Release();
    }

}
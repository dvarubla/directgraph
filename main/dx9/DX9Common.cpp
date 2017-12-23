#include "DX9Common.h"
#include "DX9Exception.h"
#include <windows.h>
#include <iostream>

namespace directgraph {
    DX9Common::DX9Common() : _d3d(NULL), _device(NULL), _d3dpp({}) {
        try {
            _d3d = Direct3DCreate9(D3D_SDK_VERSION);
            if (_d3d == NULL) {
                throw DX9Exception(std::wstring(L"Error in Direct3DCreate9"));
            }
        } catch (...) {
            throw;
        }
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
            _device->CreateAdditionalSwapChain(&_d3dpp, &swapChain);
        }
        return swapChain;
    }

    void DX9Common::createDevice(HWND win, uint_fast32_t width, uint_fast32_t height) {
        D3DDEVTYPE devType;
        DWORD flags;
        checkCaps(devType, flags);
        D3DDISPLAYMODE displayMode = {};
        _d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
        _format = displayMode.Format;
        _d3dpp.BackBufferFormat = _format;
        _d3dpp.Windowed = TRUE;
        _d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        _d3dpp.BackBufferWidth = width;
        _d3dpp.BackBufferHeight = height;

        _d3d->CreateDevice(D3DADAPTER_DEFAULT,
                           devType,
                           win,
                           flags,
                           &_d3dpp,
                           &_device
        );
        _device->SetRenderState(D3DRS_LIGHTING, FALSE);
        _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        _device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
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
        swapChain->Release();
    }

    D3DFORMAT DX9Common::getFormat() {
        return _format;
    }

}
#include "Common.h"
#include "Exception.h"
#include <windows.h>

namespace directgraph {
    namespace dx9 {
        Common::Common() : _d3d(NULL), _device(NULL) {
            _d3d = Direct3DCreate9(D3D_SDK_VERSION);
            if (_d3d == NULL) {
                THROW_EXC_CODE(
                        Exception, CANT_CREATE_DIRECTX9, std::wstring(L"Can't create DirectX object")
                );
            }
            try {
                _features = new DeviceFeatures(_d3d);
            } catch (const std::exception &){
                _d3d->Release();
                throw;
            }
            InitializeCriticalSection(&_mainCS);
        }

        void Common::getDpi(float &dpiX, float &dpiY) {
            HDC dc = GetDC(NULL);
            dpiX = (float) GetDeviceCaps(dc, LOGPIXELSX);
            dpiY = (float) GetDeviceCaps(dc, LOGPIXELSY);
        }

        IDirect3DSwapChain9 *Common::createSwapChain(HWND win, uint_fast32_t width, uint_fast32_t height) {
            IDirect3DSwapChain9 *swapChain = NULL;
            if (_device == NULL) {
                createDevice(win, width, height);
                _device->GetSwapChain(0, &swapChain);
            } else {
                _d3dpp.BackBufferWidth = width;
                _d3dpp.BackBufferHeight = height;
                _d3dpp.hDeviceWindow = win;
                if (_device->CreateAdditionalSwapChain(&_d3dpp, &swapChain) != D3D_OK) {
                    THROW_EXC_CODE(
                            Exception, DX9_CANT_CREATE_EXTRA_WIN, std::wstring(L"Can't create extra window")
                    );
                }
            }
            return swapChain;
        }

        void Common::createDevice(HWND win, uint_fast32_t width, uint_fast32_t height) {
            D3DDEVTYPE devType;
            DWORD flags;
            _features->getDeviceFlags(devType, flags);

            _format = _features->getDisplayMode();
            ZeroMemory(&_d3dpp, sizeof(_d3dpp));
            _d3dpp.BackBufferFormat = _format;
            _d3dpp.Windowed = TRUE;
            _d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
            _d3dpp.BackBufferWidth = width;
            _d3dpp.BackBufferHeight = height;

            if (_d3d->CreateDevice(D3DADAPTER_DEFAULT,
                                   devType,
                                   win,
                                   flags,
                                   &_d3dpp,
                                   &_device
            ) != D3D_OK) {
                THROW_EXC_CODE(
                        Exception, DX9_CANT_CREATE_DEVICE, std::wstring(L"Can't create DirectX device")
                );
            }
            try {
                if (_device->SetRenderState(D3DRS_LIGHTING, FALSE) != D3D_OK) {
                    THROW_EXC_CODE(
                            Exception, DX9_CANT_SET_RENDER_STATE, std::wstring(L"Can't disable lighting")
                    );
                }
                if (_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE) != D3D_OK) {
                    THROW_EXC_CODE(
                            Exception, DX9_CANT_SET_RENDER_STATE, std::wstring(L"Can't disable cull mode")
                    );
                }
            } catch (const std::exception &) {
                _device->Release();
                throw;
            }
        }

        void Common::deleteSwapChain(IDirect3DSwapChain9 *swapChain) {
            if (swapChain != NULL) {
                swapChain->Release();
            }
        }

        D3DFORMAT Common::getFormat() {
            return _format;
        }

        void Common::lock() {
            EnterCriticalSection(&_mainCS);
        }

        void Common::unlock() {
            LeaveCriticalSection(&_mainCS);
        }

        Common::~Common() {
            _device->Release();
            _d3d->Release();
            delete _features;
            DeleteCriticalSection(&_mainCS);
        }

        IFeatures *Common::getFeatures() {
            return _features;
        }

        void Common::setFeatures(IFeatures *features) {
            _features = features;
        }
    }
}
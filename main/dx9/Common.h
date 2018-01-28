#pragma once

#include <d3d9.h>
#include <stdint.h>
#include "IFeatures.h"
#include "DeviceFeatures.h"

namespace directgraph {
    namespace dx9 {
        class Common {
        private:
            IDirect3D9 *_d3d;
            IFeatures *_features;
            IDirect3DDevice9 *_device;
            D3DFORMAT _format;
            CRITICAL_SECTION _mainCS;

            void createDevice(HWND win, uint_fast32_t width, uint_fast32_t height);

            D3DPRESENT_PARAMETERS _d3dpp;
        public:
            Common();

            ~Common();

            D3DFORMAT getFormat();

            void getDpi(float &dpiX, float &dpiY);

            IDirect3DSwapChain9 *createSwapChain(HWND hwnd, uint_fast32_t width, uint_fast32_t height);

            void deleteSwapChain(IDirect3DSwapChain9 *swapChain);

            IFeatures * getFeatures();

            void setFeatures(IFeatures * features);

            void lock();

            void unlock();
        };
    }
}

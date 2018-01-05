#ifndef DIRECTGRAPH_DX9COMMON_H
#define DIRECTGRAPH_DX9COMMON_H

#include <d3d9.h>
#include <stdint.h>

namespace directgraph {
    class DX9Common {
    private:
        IDirect3D9 *_d3d;
        IDirect3DDevice9 *_device;
        D3DFORMAT _format;
        CRITICAL_SECTION _mainCS;

        void checkCaps(D3DDEVTYPE &devType, DWORD &flags);

        void createDevice(HWND win, uint_fast32_t width, uint_fast32_t height);

        D3DPRESENT_PARAMETERS _d3dpp;
    public:
        DX9Common();

        ~DX9Common();

        D3DFORMAT getFormat();

        void getDpi(float &dpiX, float &dpiY);

        IDirect3DSwapChain9 *createSwapChain(HWND hwnd, uint_fast32_t width, uint_fast32_t height);

        void deleteSwapChain(IDirect3DSwapChain9 *swapChain);

        void lock();

        void unlock();
    };
}

#endif //DIRECTGRAPH_DX9COMMON_H

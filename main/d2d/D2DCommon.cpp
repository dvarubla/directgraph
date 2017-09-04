#include <d2d1.h>
#include "D2DCommon.h"

namespace directgraph {
    D2DCommon::D2DCommon() {
        D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED,
                &_D2DFactory
        );
    }

    ID2D1Factory *D2DCommon::getFactory() {
        return _D2DFactory;
    }

    void D2DCommon::getDPI(float &x, float &y) {
        _D2DFactory->GetDesktopDpi(&x, &y);
    }
}
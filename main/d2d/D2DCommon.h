#ifndef DIRECTGRAPH_D2DCOMMON_H
#define DIRECTGRAPH_D2DCOMMON_H

#include <d2d1.h>

namespace directgraph {
    class D2DCommon {
    private:
        ID2D1Factory *_D2DFactory;
    public:
        D2DCommon();
        void getDPI(float &x, float &y);
        ID2D1Factory * getFactory();
    };
}

#endif //DIRECTGRAPH_D2DCOMMON_H

#ifndef DIRECTGRAPH_BITMAPCREATOR_H
#define DIRECTGRAPH_BITMAPCREATOR_H

#include <d2d1.h>
#include <DPIHelper.h>

namespace directgraph {
    class BufferCreator {
    private:
        DPIHelper *_helper;
        ID2D1HwndRenderTarget *_rTarget;
        ID2D1BitmapRenderTarget *_curBuffer;
        float _width, _height;
    public:
        BufferCreator(DPIHelper *helper);
        ~BufferCreator();
        void setTarget(ID2D1HwndRenderTarget *rTarget);

        ID2D1BitmapRenderTarget* getBuffer(float width, float height);
    };
}

#endif //DIRECTGRAPH_BITMAPCREATOR_H

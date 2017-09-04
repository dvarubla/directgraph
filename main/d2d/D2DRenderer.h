#ifndef DIRECTGRAPH_D2DRENDERER_H
#define DIRECTGRAPH_D2DRENDERER_H

#include <IRenderer.h>
#include <windows.h>
#include <d2d1.h>
#include <DPIHelper.h>
#include <d2d/BufferCreator.h>
#include <DPIConverter.h>
#include "D2DCommon.h"

namespace directgraph {
    class D2DRenderer : public IRenderer {
    private:
        D2DCommon *_common;
        ID2D1Factory *_D2DFactory;
        BufferCreator *_bufferCreator;
        ID2D1HwndRenderTarget *_rTarget;
        ID2D1SolidColorBrush *_solidBrush;
        ID2D1Brush *_currentBrush;
        ID2D1BitmapRenderTarget *_buffer;
        DPIHelper *_helper;
        DPIConverter *_converter;
        HWND _hwnd;
        float _width, _height;

        void createDeviceRes();

    public:
        D2DRenderer(D2DCommon *common, DPIHelper *helper, DPIConverter *converter, float width, float height);
        virtual ~D2DRenderer();

        virtual void setWindow(HWND hwnd);

        virtual void setcolor(uint_fast32_t color);

        virtual void bar(float left, float right, float top, float bottom);

        virtual void clear();

        virtual void repaint();

    };
}

#endif //DIRECTGRAPH_D2DRENDERER_H

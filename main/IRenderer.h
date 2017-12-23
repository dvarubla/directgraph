#ifndef DIRECTGRAPH_IRENDERER_H
#define DIRECTGRAPH_IRENDERER_H

#include <common.h>
#include <windows.h>
#include <DPIHelper.h>
#include <stdint.h>
#include "QueueReader.h"
#include "CommonProps.h"
#include "PixelContainerFactory.h"

namespace directgraph {
    IFACE IRenderer {
    public:
        virtual ~IRenderer(){};

        virtual void setWindow(HWND hwnd) = 0;

        virtual void repaint() = 0;

        virtual void draw(IQueueReader *reader, CommonProps *props) = 0;

        virtual PixelContainerFactory* getPixContFactory() = 0;
    };
}
#endif //DIRECTGRAPH_IRENDERER_H

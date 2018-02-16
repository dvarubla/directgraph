#pragma once

#include <common.h>
#include <windows.h>
#include <DPIHelper.h>
#include <stdint.h>
#include "QueueReader.h"
#include "CommonProps.h"
#include "PixelContainerCreator.h"

namespace directgraph {
    class IRenderer {
    public:
        virtual ~IRenderer(){};

        virtual void setWindow(HWND hwnd) = 0;

        virtual void repaint() = 0;

        virtual void prepare(IQueueReader *reader) = 0;

        virtual void draw(IQueueReader *reader) = 0;

        virtual PixelContainerCreator* getPixContFactory() = 0;
    };
}

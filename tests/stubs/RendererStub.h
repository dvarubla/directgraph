#pragma once

#include <QueueItem.h>
#include <vector>
#include <IRenderer.h>
#include <gmock/gmock.h>

namespace directgraph{
    class RendererStub: public IRenderer {
    private:
        std::vector<QueueItem> _data;
    public:

        MOCK_METHOD0(getPixContFactory, PixelContainerFactory *());

        std::vector<QueueItem> & getData();

        MOCK_METHOD1(setWindow, void (HWND hwnd));

        MOCK_METHOD0(repaint, void ());

        MOCK_METHOD1(draw, void(IQueueReader *reader));

        void drawImpl(IQueueReader *reader);
    };
}

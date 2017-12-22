#ifndef PROJECT_RENDERERSTUB_H
#define PROJECT_RENDERERSTUB_H

#include <QueueItem.h>
#include <vector>
#include <IRenderer.h>

namespace directgraph{
    class RendererStub: public IRenderer {
    private:
        std::vector<QueueItem> _data;
        CommonProps *_props;
    public:

        std::vector<QueueItem> & getData();

        void setWindow(HWND hwnd);

        void repaint();

        void draw(IQueueReader *reader, CommonProps *props);

        CommonProps *getProps();
    };
}

#endif //PROJECT_RENDERERSTUB_H

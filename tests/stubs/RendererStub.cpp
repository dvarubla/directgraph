#include "RendererStub.h"

namespace directgraph{
    void RendererStub::setWindow(HWND hwnd) {
    }

    void RendererStub::repaint() {
    }

    void RendererStub::draw(IQueueReader *reader, CommonProps *props) {
        uint_fast32_t size = reader->getSize();
        for(uint_fast32_t i = 0; i < size; i++){
            _data.push_back(reader->getAt(i));
        }
        _props = props;
        reader->endReading(size);
    }

    std::vector<QueueItem> &RendererStub::getData() {
        return _data;
    }

    CommonProps *RendererStub::getProps() {
        return _props;
    }

    PixelContainerFactory *RendererStub::getPixContFactory() {
        return NULL;
    }
}

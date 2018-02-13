#include "RendererStub.h"

namespace directgraph{

    void RendererStub::drawImpl(IQueueReader *reader) {
        uint_fast32_t size = reader->getSize();
        for(uint_fast32_t i = 0; i < size; i++){
            _data.push_back(reader->getAt(i));
        }
        reader->endReading(size);
    }

    std::vector<QueueItem> &RendererStub::getData() {
        return _data;
    }

    void RendererStub::prepareImpl(IQueueReader *reader) {
        drawImpl(reader);
    }
}

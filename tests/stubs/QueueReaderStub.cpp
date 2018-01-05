#include "QueueReaderStub.h"

namespace directgraph{
    QueueItem &QueueReaderStub::getAtImpl(uint_fast32_t index) {
        return data[index];
    }

    uint_fast32_t QueueReaderStub::getSizeImpl() {
        return static_cast<uint_fast32_t>(data.size());
    }

    void QueueReaderStub::endReadingImpl(uint_fast32_t numRead) {
        this->numRead = numRead;
        data.erase(data.begin(), data.begin() + numRead);
    }

    void QueueReaderStub::addItemsImpl(QueueItem *items, uint_fast32_t numItems) {
        for(uint_fast32_t i = 0; i < numItems; i++){
            data.push_back(items[i]);
        }
    }
}
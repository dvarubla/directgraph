#include "QueueReaderStub.h"

namespace directgraph{
    QueueItem &QueueReaderStub::getAt(uint_fast32_t index) {
        return data[index];
    }

    uint_fast32_t QueueReaderStub::getSize() {
        return data.size();
    }

    void QueueReaderStub::endReading(uint_fast32_t numRead) {
        this->numRead = numRead;
        data.erase(data.begin(), data.begin() + numRead);
    }

    void QueueReaderStub::addItems(QueueItem *items, uint_fast32_t numItems) {
        for(uint_fast32_t i = 0; i < numItems; i++){
            data.push_back(items[i]);
        }
    }
}
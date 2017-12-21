#ifndef PROJECT_QUEUEREADERSTUB_H
#define PROJECT_QUEUEREADERSTUB_H

#include <vector>
#include <main/IQueueReader.h>

namespace directgraph {
    class QueueReaderStub : public IQueueReader{
    public:
        std::vector<QueueItem> data;
        uint_fast32_t numRead;
    public:
        void addItems(QueueItem *items, uint_fast32_t numItems);

        QueueItem &getAt(uint_fast32_t index);

        uint_fast32_t getSize();

        void endReading(uint_fast32_t numRead);
    };
}

#endif //PROJECT_QUEUEREADERSTUB_H

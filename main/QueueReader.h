#ifndef PROJECT_QUEUEREADER_H
#define PROJECT_QUEUEREADER_H

#include "Queue.h"
#include "IQueueReader.h"

namespace directgraph {
    class QueueReader: public IQueueReader{
    private:
        Queue *_queue;
        uint_fast32_t _getIndex;
        uint_fast32_t _size;
        CRITICAL_SECTION *_queueCS;
        CRITICAL_SECTION *_lastElemCS;
        bool _holdingLastElemCS;
    public:
        QueueReader(Queue *queue, CRITICAL_SECTION *queueCS, CRITICAL_SECTION *lastElemCS);
        void setGetIndex(uint_fast32_t getIndex, uint_fast32_t size);
        QueueItem &getAt(uint_fast32_t index);
        uint_fast32_t getSize();
        void endReading(uint_fast32_t numRead);
    };
}

#endif //PROJECT_QUEUEREADER_H

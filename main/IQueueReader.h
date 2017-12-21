#ifndef PROJECT_IQUEUEREADER_H
#define PROJECT_IQUEUEREADER_H

#include <stdint.h>
#include "QueueItem.h"

namespace directgraph{
    class IQueueReader {
    public:
        virtual QueueItem &getAt(uint_fast32_t index) = 0;
        virtual uint_fast32_t getSize() = 0;
        virtual void endReading(uint_fast32_t numRead) = 0;
        virtual ~IQueueReader(){}
    };
}

#endif //PROJECT_IQUEUEREADER_H

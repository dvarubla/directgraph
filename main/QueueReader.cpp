#include "QueueReader.h"

namespace directgraph{

    QueueReader::QueueReader(Queue *queue, CRITICAL_SECTION *queueCS) :_queue(queue), _queueCS(queueCS) {
    }

    void QueueReader::setGetIndex(uint_fast32_t getIndex, uint_fast32_t size) {
        _getIndex = getIndex;
        _size = size;
    }

    QueueItem &QueueReader::getAt(uint_fast32_t index) {
        return _queue->getItemAt(_queue->transformIndex(index + _getIndex));
    }

    uint_fast32_t QueueReader::getSize() {
        return _size;
    }

    void QueueReader::endReading(uint_fast32_t numRead) {
        _queue->advanceGetIndex(numRead);
        LeaveCriticalSection(_queueCS);
    }
}
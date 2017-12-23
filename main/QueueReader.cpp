#include "QueueReader.h"

namespace directgraph{

    QueueReader::QueueReader(Queue *queue, CRITICAL_SECTION *queueCS, CRITICAL_SECTION *lastElemCS)
            :_queue(queue), _queueCS(queueCS), _lastElemCS(lastElemCS) {
    }

    void QueueReader::setGetIndex(uint_fast32_t getIndex, uint_fast32_t size) {
        _getIndex = getIndex;
        _size = size;
        _holdingLastElemCS = false;
    }

    QueueItem &QueueReader::getAt(uint_fast32_t index) {
        if(index + 1 == _size){
            _holdingLastElemCS = true;
            EnterCriticalSection(_lastElemCS);
        }
        return _queue->getItemAt(_queue->transformIndex(index + _getIndex));
    }

    uint_fast32_t QueueReader::getSize() {
        return _size;
    }

    void QueueReader::endReading(uint_fast32_t numRead) {
        _queue->advanceGetIndex(numRead);
        if(_holdingLastElemCS){
            LeaveCriticalSection(_lastElemCS);
        }
        LeaveCriticalSection(_queueCS);
    }
}
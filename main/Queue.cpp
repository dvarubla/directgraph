#include "Queue.h"
#include "QueueItem.h"

namespace directgraph{
    Queue::Queue():_getIndex(0),_putIndex(0),_size(SIZE_INCREMENT) {
        _items = static_cast<QueueItem *>(malloc(sizeof(QueueItem) * _size));
    }

    uint_fast32_t Queue::getGetIndex() {
        return static_cast<uint_fast32_t>(InterlockedCompareExchange(&_getIndex, 0, 0));
    }

    uint_fast32_t Queue::getPutIndex() {
        return static_cast<uint_fast32_t>(InterlockedCompareExchange(&_putIndex, 0, 0));
    }

    uint_fast32_t Queue::transformIndex(int_fast32_t index) {
        return (_size + index) % _size;
    }

    QueueItem &Queue::getItemAt(uint_fast32_t index) {
        return _items[index];
    }

    void Queue::writeItem(const QueueItem &item) {
        _items[getPutIndex()] = item;
        uint_fast32_t putIndex = getPutIndex();
        InterlockedExchange(&_putIndex, static_cast<LONG>(transformIndex((putIndex + 1))));
    }

    void Queue::grow() {
        uint_fast32_t oldSize = _size;
        _size += SIZE_INCREMENT;
        QueueItem *newItems = static_cast<QueueItem *>(malloc(sizeof(QueueItem) * _size));
        if (_putIndex >= _getIndex) {
            memcpy(newItems, _items + _getIndex, static_cast<size_t>(_putIndex - _getIndex) * sizeof(QueueItem));
            _putIndex -= _getIndex;
        } else {
            uint_fast32_t sizeTillEnd = oldSize - _getIndex;
            memcpy(newItems, _items + _getIndex, sizeTillEnd * sizeof(QueueItem));
            memcpy(newItems + sizeTillEnd, _items, (size_t) (_putIndex * sizeof(QueueItem)));
            _putIndex += sizeTillEnd;
        }
        _getIndex = 0;
        free(_items);
        _items = newItems;
    }

    bool Queue::needGrow() {
        return transformIndex(getPutIndex() + 1) == getGetIndex();
    }

    uint_fast32_t Queue::getReadSize() {
        return transformIndex(getPutIndex() - getGetIndex());
    }

    void Queue::advanceGetIndex(uint_fast32_t num) {
        uint_fast32_t getIndex = getGetIndex();
        InterlockedExchange(&_getIndex, static_cast<LONG>(transformIndex((getIndex + num))));
    }

    Queue::~Queue() {
        uint_fast32_t size = getReadSize();
        for(uint_fast32_t i = 0; i < size; i++){
            QueueItem &item = getItemAt(transformIndex(i + _getIndex));
            if(item.type == QueueItem::SETFILLPATTERN){
                delete [] item.data.setfillpattern.fillPattern;
            }
        }
        free(_items);
    }
}
#ifndef PROJECT_QUEUE_H
#define PROJECT_QUEUE_H

#include <stdint.h>
#include <windows.h>

namespace directgraph {
    struct QueueItem{
        enum {
            CLEAR = 0,
            SETCOLOR,
            BAR
        } type;
        union{
            struct{
                uint32_t color;
            } setcolor;
            struct {
                float left, top, right, bottom;
            } bar;
        } data;
    };
    class Queue {
    private:
        QueueItem *_items;
        LONG volatile _getIndex;
        LONG volatile _putIndex;
        uint_fast32_t _size;
    public:
        static const uint_fast32_t SIZE_INCREMENT = 128;
        Queue();
        ~Queue();
        uint_fast32_t getGetIndex();
        uint_fast32_t getPutIndex();
        uint_fast32_t transformIndex(uint_fast32_t index);
        QueueItem& getItemAt(uint_fast32_t index);
        void writeItem(const QueueItem &item);
        void advanceGetIndex(uint_fast32_t num);
        void grow();
        bool needGrow();
        uint_fast32_t getReadSize();
    };
}

#endif //PROJECT_QUEUE_H

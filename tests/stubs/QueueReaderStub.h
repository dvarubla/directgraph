#pragma once

#include <vector>
#include <main/IQueueReader.h>
#include <gmock/gmock.h>

namespace directgraph {
    class QueueReaderStub : public IQueueReader{
    public:
        std::vector<QueueItem> data;
        uint_fast32_t numRead;
    public:
        MOCK_METHOD2(addItems, void(QueueItem *items, uint_fast32_t numItems));
        
        void addItemsImpl(QueueItem *items, uint_fast32_t numItems);
        
        MOCK_METHOD1(getAt, QueueItem &(uint_fast32_t index));
        
        QueueItem &getAtImpl(uint_fast32_t index);

        MOCK_METHOD0(getSize, uint_fast32_t());
        uint_fast32_t getSizeImpl();

        MOCK_METHOD1(endReading, void (uint_fast32_t numRead));
        
        void endReadingImpl(uint_fast32_t numRead);
    };
}
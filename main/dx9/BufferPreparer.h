#pragma once

#include <stdint.h>
#include <main/IQueueReader.h>
#include <vector>
#include <unordered_map>
#include <main/CommonProps.h>
#include "ShaderManager.h"
#include "BufferPreparerParams.h"
#include "StateHelper.h"
#include "PropertyManager.h"
#include "DrawOpCreator.h"
#include "DrawItemProcessor.h"
#include "DrawerManager.h"

namespace directgraph {
    namespace dx9 {
        class BufferPreparer {
        public:
            typedef std::vector<char*> CharPVector;
            typedef std::vector<uint8_t> ByteVector;
            typedef std::vector<uint32_t> OffsetVector;
            struct MemBlock{
                uint8_t *mem;
                uint_fast32_t size;
            };
            typedef std::vector<MemBlock> MemBlockVector;
            struct ItemsBuffer{
                ByteVector vect;
                OffsetVector offsets;
                uint32_t numItems;
                DrawDataType::Type type;
                Coords prevCrds;
            };

            const static int VERTICES_TRIANGLES_DIFF = 2;
        private:
            typedef std::unordered_map<ItemState, ItemsBuffer, ItemStateHash> BufferMap;
            struct {
                ByteVector vect;

                uint32_t numItems;
                DrawOpVector drawOps;
                Coords prevCrds;
            } _transpBuffer;

            BufferPreparerParams *_bufPrepParams;
            uint_fast32_t _memSize;
            BufferMap _drawBuffers;
            DrawOpVector _drawOps;
            MemBlockVector _memBlocks;
            CharPVector _patterns;
            StateHelper _stateHelper;
            PropertyManager _propMan;
            DrawerManager _drawerManager;
            DrawItemProcessor _drawItemProc;
            bool _isFirst;
            uint_fast32_t _curUsedSize;
            uint_fast32_t _lastOffset;
            bool _canReadMore;
        public:
            BufferPreparer(
                    uint_fast32_t memSize,
                    const CommonProps &props,
                    BufferPreparerParams *bufPrepParams
            );
            ~BufferPreparer();
            void prepareBuffer(IQueueReader *reader, uint_fast32_t offset, uint_fast32_t maxSize);
            void clear();
            void genOpsAndMemBlocks();
            DrawOpVector::iterator drawOpsBegin();
            DrawOpVector::iterator drawOpsEnd();

            MemBlockVector::iterator memBlocksBegin();
            MemBlockVector::iterator memBlocksEnd();
            bool isEmpty();
            uint_fast32_t getUsedSize();
            bool isFull();
            uint_fast32_t getLastOffset();
            void resetLastOffset();
        };
    }
}



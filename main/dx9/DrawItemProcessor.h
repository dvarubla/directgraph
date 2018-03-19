#pragma once

#include "StateHelper.h"
#include "DrawOpCreator.h"
#include "BufferPreparerParams.h"
#include "PrimitiveCreator.h"
#include "misc.h"
#include <main/QueueItem.h>
#include <main/DPIHelper.h>

namespace directgraph {
    namespace dx9 {
        class DrawItemProcessor {
        private:
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            PrimitiveCreator _primCreator;
            float _curZ;
            uint_fast32_t _itemNum;

            void setCurZ();
        public:
            DrawItemProcessor(StateHelper *stateHelper, BufferPreparerParams *bufPrepParams);
            struct TypeSize{
                uint_fast32_t sizeMult;
                DrawDataType::Type drawDataType;
            };
            struct NumVertices{
                uint_fast32_t degenerate;
                uint_fast32_t primitive;
            };
            bool canCreateMoreItems();
            void nextItem();
            void resetItemCount();
            NumVertices getNumVertices(const QueueItem &item, bool isFirst);
            TypeSize getTypeSize(const QueueItem &item, const ItemState &state);
            void processDrawItem(
                    const QueueItem &item, void *&curVertMem,
                    uint_fast32_t &numVertices,
                    const ItemState &state
            );
            StartEndCoords getStartEndCoords(
                    const QueueItem &item, const ItemState &state
            );

            void genDegenerates(
                    const QueueItem &item, void *&curVertMem,
                    const Coords &startCrds, const Coords &endCrds,
                    const ItemState &state
            );
        };
    }
}


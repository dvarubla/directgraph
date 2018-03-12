#pragma once

#include "StateHelper.h"
#include "DrawOpCreator.h"
#include "BufferPreparerParams.h"
#include "PrimitiveCreator.h"
#include <main/QueueItem.h>
#include <main/DPIHelper.h>

namespace directgraph {
    namespace dx9 {
        class DrawItemProcessor {
        private:
            const static int TRIANGLES_IN_QUAD = 2;
            const static int VERTICES_IN_QUAD = 4;

            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            DPIHelper *_helper;
            PrimitiveCreator _primCreator;
            float _curZ;
            uint_fast32_t _itemNum;

            void setCurZ();
        public:
            DrawItemProcessor(StateHelper *stateHelper, BufferPreparerParams *bufPrepParams, DPIHelper *dpiHelper);
            struct TypeSize{
                uint_fast32_t sizeMult;
                DrawDataType::Type drawDataType;
            };
            struct StartEndCoords{
                Coords start;
                Coords end;
            };
            bool canCreateMoreItems();
            void nextItem();
            void resetItemCount();
            uint_fast32_t getNumVertices(const QueueItem &item, bool isFirst);
            TypeSize getTypeSize(const QueueItem &item, const ItemState &state);
            void processDrawItem(
                    const QueueItem &item, void *&curVertMem,
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


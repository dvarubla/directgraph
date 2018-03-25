#pragma once

#include <primitiveHelpers/DegenerateHelper.h>
#include <primitiveHelpers/SimplePrimHelper.h>
#include <BufferPreparerParams.h>
#include <DrawStateHelper.h>
#include <primitiveHelpers/RectangleHelper.h>
#include "IDrawer.h"

namespace directgraph {
    namespace dx9 {
        class RectangleDrawer: public IDrawer {
        private:
            DrawStateHelper *_drawStateHelper;
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degenerateHelper;
            RectangleHelper *_rectangleHelper;
            QueueItem _curItem;
            ItemState _curState;
        public:
            RectangleDrawer(
                    DrawStateHelper *drawStateHelper,
                    StateHelper *stateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan,
                    SimplePrimHelper *simplePrimHelper,
                    DegenerateHelper *degenerateHelper,
                    RectangleHelper *rectangleHelper
            );
            void getItemState(ItemState &state);

            NumVertices getNumVertices(bool isFirst);

            TypeSize getTypeSize();

            void processDrawItem(void *&curVertMem, uint_fast32_t &numVertices, float curZ);

            StartEndCoords getStartEndCoords();

            void
            genDegenerates(void *&curVertMem, const Coords &startCrds, const Coords &endCrds, float curZ);

            bool isSemiTransparent();

            ~RectangleDrawer();

            void setItemState(const ItemState &state);
            void setItem(const QueueItem &item);
        };
    }
}



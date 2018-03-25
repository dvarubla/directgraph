#pragma once

#include <DrawStateHelper.h>
#include <BufferPreparerParams.h>
#include <primitiveHelpers/SimplePrimHelper.h>
#include <primitiveHelpers/DegenerateHelper.h>
#include <primitiveHelpers/EllipseHelper.h>
#include "IDrawer.h"

namespace directgraph {
    namespace dx9 {
        class EllipseDrawer: public IDrawer{
        private:
            DrawStateHelper *_drawStateHelper;
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            EllipseHelper *_ellipseHelper;
            DegenerateHelper *_degenerateHelper;
            QueueItem _curItem;
            ItemState _curState;
        public:
            EllipseDrawer(
                    DrawStateHelper *drawStateHelper,
                    StateHelper *stateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan,
                    SimplePrimHelper *simplePrimHelper,
                    EllipseHelper *ellipseHelper,
                    DegenerateHelper *degenerateHelper
            );

            void getItemState(ItemState &state);

            NumVertices getNumVertices(bool isFirst);

            TypeSize getTypeSize();

            void processDrawItem(void *&curVertMem, uint_fast32_t &numVertices, float curZ);

            StartEndCoords getStartEndCoords();

            void
            genDegenerates(void *&curVertMem, const Coords &startCrds, const Coords &endCrds,
                           float curZ);

            bool isSemiTransparent();

            ~EllipseDrawer();

            void setItemState(const ItemState &state);
            void setItem(const QueueItem &item);
        };
    }
}


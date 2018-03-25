#pragma once

#include <DrawStateHelper.h>
#include <BufferPreparerParams.h>
#include <primitiveHelpers/SimplePrimHelper.h>
#include <primitiveHelpers/DegenerateHelper.h>
#include "NullDrawer.h"

namespace directgraph {
    namespace dx9 {
        class SinglePixelDrawer: public NullDrawer{
        private:
            DrawStateHelper *_drawStateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degenerateHelper;
            QueueItem _curItem;
        public:
            SinglePixelDrawer(
                    DrawStateHelper *drawStateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan,
                    SimplePrimHelper *simplePrimHelper,
                    DegenerateHelper *degenerateHelper
            );

            void getItemState(ItemState &state);

            NumVertices getNumVertices(bool isFirst);

            TypeSize getTypeSize();

            void processDrawItem(void *&curVertMem, uint_fast32_t &numVertices, float curZ);

            StartEndCoords getStartEndCoords();

            void genDegenerates(void *&curVertMem, const Coords &startCrds, const Coords &endCrds, float curZ);

            ~SinglePixelDrawer();

            void setItem(const QueueItem &item);
        };
    }
}


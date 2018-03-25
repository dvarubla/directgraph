#pragma once

#include <primitiveHelpers/SimplePrimHelper.h>
#include <BufferPreparerParams.h>
#include <DrawStateHelper.h>
#include "NullDrawer.h"

namespace directgraph {
    namespace dx9 {
        class PixelContainerDrawer: public NullDrawer {
        private:
            DrawStateHelper *_drawStateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            QueueItem _curItem;
        public:
            PixelContainerDrawer(
                    DrawStateHelper *drawStateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan,
                    SimplePrimHelper *simplePrimHelper
            );

            void getItemState(ItemState &state);

            NumVertices getNumVertices(bool isFirst);

            TypeSize getTypeSize();

            void processDrawItem(void *&curVertMem, uint_fast32_t &numVertices, float curZ);

            StartEndCoords getStartEndCoords();

            void genDegenerates(void *&curVertMem, const Coords &startCrds, const Coords &endCrds, float curZ);

            void setItem(const QueueItem &item);

            bool isSemiTransparent();

            ~PixelContainerDrawer();
        };
    }
}



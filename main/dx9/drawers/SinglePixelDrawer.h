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
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degenerateHelper;
            QueueItem _curItem;
            FCoords _coords[2];
            void genPixelCoords();
        public:
            SinglePixelDrawer(
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

            void genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds, float curZ);

            ~SinglePixelDrawer();

            void setItem(const QueueItem &item);
        };
    }
}


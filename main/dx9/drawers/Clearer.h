#pragma once

#include <primitiveHelpers/DegenerateHelper.h>
#include <primitiveHelpers/SimplePrimHelper.h>
#include <BufferPreparerParams.h>
#include <StateHelper.h>
#include <DrawStateHelper.h>
#include "NullDrawer.h"

namespace directgraph {
    namespace dx9 {
        class Clearer: public NullDrawer {
        private:
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degenerateHelper;
        public:
            Clearer(
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

            void
            genDegenerates(void *&curVertMem, const Coords &startCrds, const Coords &endCrds,
                           float curZ);

            ~Clearer();
        };
    }
}



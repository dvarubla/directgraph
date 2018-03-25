#pragma once

#include <DrawStateHelper.h>
#include <BufferPreparerParams.h>
#include <primitiveHelpers/SimplePrimHelper.h>
#include <primitiveHelpers/DegenerateHelper.h>
#include <TextureCoordsCalc.h>
#include "NullDrawer.h"

namespace directgraph {
    namespace dx9 {
        class BarDrawer : public NullDrawer {
        private:
            DrawStateHelper *_drawStateHelper;
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degenerateHelper;
            TextureCoordsCalc *_texCrdCalc;
            QueueItem _curItem;
            ItemState _curState;
        public:
            BarDrawer(
                    DrawStateHelper *drawStateHelper,
                    StateHelper *stateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan,
                    SimplePrimHelper *simplePrimHelper,
                    DegenerateHelper *degenerateHelper,
                    TextureCoordsCalc *texCrdCalc
            );
            void getItemState(ItemState &state);

            NumVertices getNumVertices(bool isFirst);

            TypeSize getTypeSize();

            void processDrawItem(
                    void *&curVertMem, uint_fast32_t &numVertices,
                    float curZ
            );

            StartEndCoords getStartEndCoords();

            void genDegenerates(
                    void *&curVertMem,
                    const Coords &startCrds, const Coords &endCrds,
                    float curZ
            );

            bool isSemiTransparent();

            ~BarDrawer();

            void setItemState(const ItemState &state);

            void setItem(const QueueItem &item);
        };
    }
}



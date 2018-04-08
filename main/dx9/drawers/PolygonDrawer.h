#pragma once

#include <DrawStateHelper.h>
#include <BufferPreparerParams.h>
#include <primitiveHelpers/SimplePrimHelper.h>
#include <primitiveHelpers/DegenerateHelper.h>
#include <TextureCoordsCalc.h>
#include <primitiveHelpers/PolygonHelper.h>
#include "NullDrawer.h"

namespace directgraph{
    namespace dx9 {
        class PolygonDrawer: public NullDrawer {
        private:
            DrawStateHelper *_drawStateHelper;
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degenerateHelper;
            TextureCoordsCalc *_texCrdCalc;
            PolygonHelper *_polygonHelper;
            QueueItem _curItem;
            ItemState _curState;
            Polyline _polyline;
        public:
            PolygonDrawer(
                    DrawStateHelper *drawStateHelper,
                    StateHelper *stateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan,
                    SimplePrimHelper *simplePrimHelper,
                    DegenerateHelper *degenerateHelper,
                    TextureCoordsCalc *texCrdCalc,
                    PolygonHelper *polygonHelper
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
                    const FCoords &startCrds, const FCoords &endCrds,
                    float curZ
            );

            bool isSemiTransparent();

            ~PolygonDrawer();

            void setItemState(const ItemState &state);

            void setItem(const QueueItem &item);
        };
    }
}



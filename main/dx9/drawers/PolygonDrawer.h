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
            ItemState _outlineState;
            ItemState _fillState;
            TypeSize _outlineTypeSize;
            TypeSize _fillTypeSize;
            Polyline _polyline;
            Polygon _polygon;
            bool _haveOutline;
            bool _haveFill;
            enum PolyStage{
                OUTLINE_STAGE = 0,
                FILL_STAGE
            } _curStage;
            void createOutlineState();
            void createFillState();
            void createOutlineTypeSize();
            void createFillTypeSize();
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

            uint_fast8_t getNumStages();

            void setStage(uint_fast8_t stage);

            uint_fast32_t getTotalSize();
        };
    }
}



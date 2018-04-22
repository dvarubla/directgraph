#pragma once

#include <primitiveHelpers/Bar3DHelper.h>
#include "NullDrawer.h"
#include "BarDrawer.h"
#include "RectangleDrawer.h"

namespace directgraph {
    namespace dx9 {
        class Bar3DDrawer: public NullDrawer {
        private:
            DrawStateHelper *_drawStateHelper;
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
            SimplePrimHelper *_simplePrimHelper;
            DegenerateHelper *_degenerateHelper;
            Bar3DHelper *_bar3DHelper;
            TextureCoordsCalc *_texCrdCalc;
            BarDrawer *_barDrawer;
            RectangleDrawer *_rectDrawer;
            IDrawer *_curDrawer;
            enum Bar3DStage{
                BAR_STAGE = 0,
                RECT_STAGE,
                SIDES_STAGE
            } _curStage;
            QueueItem _curItem;
            ItemState _curState;
            TypeSize _sidesTypeSize;
            bool _haveLine;
            bool _haveSides;
            ItemState _rectState;
            ItemState _barState;
            ItemState _sidesState;

            bool drawingSides();
            bool haveTop();
            void createSidesTypeSize();
            void createSidesState();
            uint_fast32_t getNumSidesVerts();
        public:
            Bar3DDrawer(
                    DrawStateHelper *drawStateHelper,
                    StateHelper *stateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan,
                    SimplePrimHelper *simplePrimHelper,
                    DegenerateHelper *degenerateHelper,
                    Bar3DHelper *bar3DHelper,
                    TextureCoordsCalc *texCrdCalc,
                    BarDrawer *barDrawer,
                    RectangleDrawer *rectDrawer
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

            ~Bar3DDrawer();

            void setItemState(const ItemState &state);

            void setItem(const QueueItem &item);

            uint_fast8_t getNumStages();

            void setStage(uint_fast8_t stage);

            uint_fast32_t getTotalSize();
        };
    }
}



#pragma once

#include <DrawStateHelper.h>
#include <BufferPreparerParams.h>
#include <primitiveHelpers/SimplePrimHelper.h>
#include <primitiveHelpers/DegenerateHelper.h>
#include <primitiveHelpers/EllipseHelper.h>
#include "NullDrawer.h"

namespace directgraph {
    namespace dx9 {
        class EllipseDrawer: public NullDrawer{
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
            Ellipse _ellipse;
            EllipseOutline _ellipseOutline;
            TypeSize _outlineTypeSize;
            TypeSize _fillTypeSize;
            bool _haveOutline;
            bool _haveFill;
            ItemState _fillState;
            ItemState _outlineState;

            enum PolyStage{
                OUTLINE_STAGE = 0,
                FILL_STAGE
            } _curStage;

            void createFillState();
            void createOutlineState();
            void createOutlineTypeSize();
            void createFillTypeSize();
            uint_fast32_t getNumFillVertices();
            uint_fast32_t getNumOutlineVertices();
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
            genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds,
                           float curZ);

            bool isSemiTransparent();

            ~EllipseDrawer();

            void setItemState(const ItemState &state);
            void setItem(const QueueItem &item);

            uint_fast8_t getNumStages();

            void setStage(uint_fast8_t stage);

            uint_fast32_t getTotalSize();
        };
    }
}


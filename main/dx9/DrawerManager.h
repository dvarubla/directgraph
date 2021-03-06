#pragma once

#include <drawers/IDrawer.h>
#include "primitiveHelpers/DegenerateHelper.h"
#include "DrawStateHelper.h"
#include "TextureCoordsCalc.h"
#include "primitiveHelpers/SimplePrimHelper.h"
#include "primitiveHelpers/RectangleHelper.h"
#include "primitiveHelpers/EllipseHelper.h"
#include "primitiveHelpers/LineHelper.h"
#include "primitiveHelpers/PolygonHelper.h"
#include <array>
#include <primitiveHelpers/Bar3DHelper.h>

namespace directgraph {
    namespace dx9 {
        class DrawerManager {
        private:
            enum DrawerType{
                CLEARER,
                BAR_DRAWER,
                RECTANGLE_DRAWER,
                ELLIPSE_DRAWER,
                PIXEL_CONTAINER_DRAWER,
                SINGLE_PIXEL_DRAWER,
                LINE_DRAWER,
                POLYGON_DRAWER,
                BAR3D_DRAWER,
                NUM_DRAWERS
            };
            SimplePrimHelper _simplePrimHelper;
            TextureCoordsCalc _texCrdsCalc;
            DegenerateHelper _degenerateHelper;
            TextureCoordsCalc _texCrdCalc;
            DrawStateHelper _drawStateHelper;
            RectangleHelper _rectangleHelper;
            EllipseHelper _ellipseHelper;
            LineHelper _lineHelper;
            PolygonHelper _polygonHelper;
            Bar3DHelper _bar3DHelper;

            std::array<IDrawer*, NUM_DRAWERS> _drawers;
            DrawerType _activeDrawer;
        public:
            DrawerManager(
                    StateHelper *stateHelper,
                    BufferPreparerParams *bufPrepParams,
                    PropertyManager *propMan
            );
            void setActiveDrawerType(const QueueItem &item);
            IDrawer *getActiveDrawer();
            ~DrawerManager();
        };
    }
}



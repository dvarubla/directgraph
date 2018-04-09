#include <drawers/BarDrawer.h>
#include <drawers/Clearer.h>
#include <drawers/RectangleDrawer.h>
#include <drawers/EllipseDrawer.h>
#include <drawers/PixelContainerDrawer.h>
#include <drawers/SinglePixelDrawer.h>
#include <drawers/LineDrawer.h>
#include <drawers/PolygonDrawer.h>
#include "DrawerManager.h"

namespace directgraph{
    namespace dx9{
        DrawerManager::DrawerManager(
                StateHelper *stateHelper,
                BufferPreparerParams *bufPrepParams,
                PropertyManager *propMan
        ) : _drawStateHelper(stateHelper, propMan),
            _rectangleHelper(&_simplePrimHelper, &_texCrdsCalc), _polygonHelper(&_lineHelper, &_texCrdCalc) {
            _drawers[CLEARER] = new Clearer(
                    bufPrepParams, propMan, &_simplePrimHelper, &_degenerateHelper
            );
            _drawers[BAR_DRAWER] = new BarDrawer(
                &_drawStateHelper, stateHelper, bufPrepParams, propMan,
                &_simplePrimHelper, &_degenerateHelper, &_texCrdCalc
            );
            _drawers[RECTANGLE_DRAWER] = new RectangleDrawer(
                    &_drawStateHelper, stateHelper, bufPrepParams, propMan,
                    &_simplePrimHelper, &_degenerateHelper, &_rectangleHelper
            );
            _drawers[ELLIPSE_DRAWER] = new EllipseDrawer(
                    &_drawStateHelper, stateHelper, bufPrepParams, propMan,
                    &_simplePrimHelper, &_ellipseHelper, &_degenerateHelper
            );
            _drawers[PIXEL_CONTAINER_DRAWER] = new PixelContainerDrawer(
                    bufPrepParams, propMan, &_simplePrimHelper
            );
            _drawers[SINGLE_PIXEL_DRAWER] = new SinglePixelDrawer(
                    bufPrepParams, propMan,
                    &_simplePrimHelper, &_degenerateHelper
            );
            _drawers[LINE_DRAWER] = new LineDrawer(
                    &_drawStateHelper, stateHelper, bufPrepParams, propMan,
                    &_simplePrimHelper, &_degenerateHelper, &_texCrdCalc, &_lineHelper
            );
            _drawers[POLYGON_DRAWER] = new PolygonDrawer(
                    &_drawStateHelper, stateHelper, bufPrepParams, propMan,
                    &_simplePrimHelper, &_degenerateHelper, &_texCrdCalc, &_polygonHelper
            );
        }

        void DrawerManager::setActiveDrawerType(const QueueItem &item) {
            switch (item.type){
                case QueueItem::BAR:
                    _activeDrawer = BAR_DRAWER;
                    break;
                case QueueItem::CLEAR:
                    _activeDrawer = CLEARER;
                    break;
                case QueueItem::RECTANGLE:
                    _activeDrawer = RECTANGLE_DRAWER;
                    break;
                case QueueItem::FILLELLIPSE:
                    _activeDrawer = ELLIPSE_DRAWER;
                    break;
                case QueueItem::PIXEL_CONTAINER:
                    _activeDrawer = PIXEL_CONTAINER_DRAWER;
                    break;
                case QueueItem::SINGLE_PIXEL:
                    _activeDrawer = SINGLE_PIXEL_DRAWER;
                    break;
                case QueueItem::LINE:
                    _activeDrawer = LINE_DRAWER;
                    break;
                case QueueItem::DRAWPOLY:
                    _activeDrawer = POLYGON_DRAWER;
                    break;
                default: break;
            }
        }

        IDrawer *DrawerManager::getActiveDrawer() {
            return _drawers[_activeDrawer];
        }

        DrawerManager::~DrawerManager() {
            for(uint_fast16_t i = 0; i < NUM_DRAWERS; i++){
                delete _drawers[i];
            }
        }
    }
}
#include <VertexCreator.h>
#include <main/QueueItem.h>
#include "PolygonDrawer.h"

namespace directgraph {
    namespace dx9 {

        void PolygonDrawer::getItemState(ItemState &state) {
            _drawStateHelper->useLineStyle(state, _bufPrepParams->needRecreateTexture() && !_bufPrepParams->supportsTexturedLine());
            if(_stateHelper->lineTextureUsed(state)){
                if(_bufPrepParams->supportsTexturedLine()){
                    _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_LINE_SHADER);
                }
            }
        }

        NumVertices PolygonDrawer::getNumVertices(bool isFirst) {
            NumVertices res = {(isFirst) ? 0u : 2u, _points.size()};
            return res;
        }

        TypeSize PolygonDrawer::getTypeSize() {
            TypeSize res;
            if (_stateHelper->lineTextureUsed(_curState)) {
                if(_bufPrepParams->supportsTexturedLine()) {
                    res.sizeMult = sizeof(ColorVertex);
                    res.drawDataType = DrawDataType::TEXTURED_LINE_VERTEX;
                } else {
                    res.sizeMult = sizeof(TexturedColorVertex);
                    res.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                }
            } else {
                res.sizeMult = sizeof(ColorVertex);
                res.drawDataType = DrawDataType::COLOR_VERTEX;
            }
            return res;
        }

        void PolygonDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &, float curZ) {
            curVertMem = _simplePrimHelper->genTriangles(
                    curVertMem, _points, curZ, _stateHelper->getLastState().drawColor
            );
        }

        StartEndCoords PolygonDrawer::getStartEndCoords() {
            StartEndCoords res = {
                    _points.front(),
                    _points.back()
            };
            return res;
        }

        void
        PolygonDrawer::genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds, float curZ) {
            curVertMem = _degenerateHelper->genDegenerate(
                    curVertMem, startCrds, endCrds, curZ
            );
        }

        bool PolygonDrawer::isSemiTransparent() {
            return color_has_alpha(_stateHelper->getLastState().drawColor) ||
                   (_stateHelper->lineTextureUsed(_curState) && !_bufPrepParams->supportsTexturedLine());
        }

        PolygonDrawer::~PolygonDrawer() {

        }

        void PolygonDrawer::setItemState(const ItemState &state) {
            _curState = state;
            _points = _polygonHelper->calcPolygonBorder(
                    _curItem.data.drawPoly.numPoints, _curItem.data.drawPoly.points,
                    _stateHelper->getLastState().lineThickness
            );
        }

        void PolygonDrawer::setItem(const QueueItem &item) {
            _curItem = item;
        }

        PolygonDrawer::PolygonDrawer(DrawStateHelper *drawStateHelper, StateHelper *stateHelper,
                                     BufferPreparerParams *bufPrepParams, PropertyManager *propMan,
                                     SimplePrimHelper *simplePrimHelper, DegenerateHelper *degenerateHelper,
                                     TextureCoordsCalc *texCrdCalc, PolygonHelper *polygonHelper)
                :_drawStateHelper(drawStateHelper), _stateHelper(stateHelper), _bufPrepParams(bufPrepParams),
                 _propMan(propMan), _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper),
                 _texCrdCalc(texCrdCalc), _polygonHelper(polygonHelper) {

        }
    }
}

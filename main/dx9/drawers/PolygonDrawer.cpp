#include <VertexCreator.h>
#include <main/QueueItem.h>
#include <misc.h>
#include "PolygonDrawer.h"

namespace directgraph {
    namespace dx9 {

        void PolygonDrawer::getItemState(ItemState &state) {
            if(_curStage == FILL_STAGE || !_haveOutline){
                state = _fillState;
            } else {
                state = _outlineState;
            }
        }

        void PolygonDrawer::createOutlineState() {
            _outlineState = _propMan->getNullState();
            _drawStateHelper->useLineStyle(_outlineState, _bufPrepParams->needRecreateTexture() && !_bufPrepParams->supportsTexturedLine());

            if(_stateHelper->lineTextureUsed(_outlineState)){
                if(_bufPrepParams->supportsTexturedLine()){
                    _propMan->setProp(_outlineState, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_LINE_SHADER);
                }
            }
        }

        void PolygonDrawer::createFillState() {
            _fillState = _propMan->getNullState();
            _drawStateHelper->useFillTexture(
                    _fillState,
                    !_bufPrepParams->supportsTexturedBar(), _bufPrepParams->needRecreateTexture()
            );
            if(_bufPrepParams->supportsTexturedBar() && _stateHelper->fillTextureUsed(_fillState)){
                _propMan->setProp(_fillState, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_BAR_SHADER);
            }
        }

        void PolygonDrawer::createOutlineTypeSize() {
            if (_stateHelper->lineTextureUsed(_outlineState)) {
                if(_bufPrepParams->supportsTexturedLine()) {
                    _outlineTypeSize.sizeMult = sizeof(ColorVertex);
                    _outlineTypeSize.drawDataType = DrawDataType::TEXTURED_LINE_VERTEX;
                } else {
                    _outlineTypeSize.sizeMult = sizeof(TexturedColorVertex);
                    _outlineTypeSize.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                }
            } else {
                _outlineTypeSize.sizeMult = sizeof(ColorVertex);
                _outlineTypeSize.drawDataType = DrawDataType::COLOR_VERTEX;
            }
        }

        void PolygonDrawer::createFillTypeSize() {
            if (_stateHelper->fillTextureUsed(_fillState)) {
                if(_bufPrepParams->supportsTexturedBar()){
                    _fillTypeSize.sizeMult = sizeof(Color2VertexNoRHW);
                    _fillTypeSize.drawDataType = DrawDataType::COLOR2_VERTEX;
                } else {
                    _fillTypeSize.sizeMult = sizeof(TexturedColorVertex);
                    _fillTypeSize.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                }
            } else {
                _fillTypeSize.sizeMult = sizeof(ColorVertex);
                _fillTypeSize.drawDataType = DrawDataType::COLOR_VERTEX;
            }
        }

        NumVertices PolygonDrawer::getNumVertices(bool isFirst) {
            NumVertices res;
            res.degenerate = (isFirst) ? 0u : 2u;
            if(_curStage == FILL_STAGE || !_haveOutline){
                res.primitive = static_cast<uint_fast32_t>(_polygon.coords.size());
            } else {
                res.primitive = static_cast<uint_fast32_t>(_polyline.coords.size());
            }
            return res;
        }

        TypeSize PolygonDrawer::getTypeSize() {
            if(_curStage == FILL_STAGE || !_haveOutline){
                return _fillTypeSize;
            } else {
                return _outlineTypeSize;
            }
        }

        void PolygonDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &, float curZ) {
            if(_curStage == FILL_STAGE || !_haveOutline) {
                if (_stateHelper->fillTextureUsed(_curState)) {
                    if(_bufPrepParams->supportsTexturedBar()){
                        curVertMem = _simplePrimHelper->genFillCol2Triangles(curVertMem,
                                                                        _polygon.coords, _polygon.texCoords,
                                                                        curZ,
                                                                        _stateHelper->getLastState().fillColor,
                                                                        _stateHelper->getLastState().bgColor
                        );
                    } else {
                        curVertMem = _simplePrimHelper->genTexTriangles(curVertMem,
                                                                        _polygon.coords, _polygon.texCoords,
                                                                        curZ,
                                                                        _stateHelper->getLastState().fillColor
                        );
                    }
                } else {
                    curVertMem = _simplePrimHelper->genTriangles(
                            curVertMem, _polygon.coords, curZ, _stateHelper->getFillColor()
                    );
                }
            } else {
                if (_stateHelper->lineTextureUsed(_curState)) {
                    if (_bufPrepParams->supportsTexturedLine()) {
                        curVertMem = _simplePrimHelper->genTrianglesExtra(
                                curVertMem, _polyline.coords, _polyline.texCoords, curZ,
                                _stateHelper->getLastState().drawColor
                        );
                    } else {
                        curVertMem = _simplePrimHelper->genTexTriangles(
                                curVertMem, _polyline.coords, _polyline.texCoords, curZ,
                                _stateHelper->getLastState().drawColor
                        );
                    }
                } else {
                    curVertMem = _simplePrimHelper->genTriangles(
                            curVertMem, _polyline.coords, curZ, _stateHelper->getLastState().drawColor
                    );
                }
            }
        }

        StartEndCoords PolygonDrawer::getStartEndCoords() {
            if(_curStage == FILL_STAGE || !_haveOutline) {
                StartEndCoords res = {
                        _polygon.coords.front(),
                        _polygon.coords.back()
                };
                return res;
            } else {
                StartEndCoords res = {
                        _polyline.coords.front(),
                        _polyline.coords.back()
                };
                return res;
            }
        }

        void
        PolygonDrawer::genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds, float curZ) {
            if(_curStage == FILL_STAGE || !_haveOutline) {
                if (_stateHelper->fillTextureUsed(_curState)) {
                    if (_bufPrepParams->supportsTexturedBar()) {
                        curVertMem = _degenerateHelper->genTexCol2Degenerate(
                                curVertMem, startCrds, endCrds,
                                curZ
                        );
                    } else {
                        curVertMem = _degenerateHelper->genTexDegenerate(
                                curVertMem, startCrds, endCrds, curZ
                        );
                    }
                } else {
                    curVertMem = _degenerateHelper->genDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                }
            } else {
                if (_stateHelper->lineTextureUsed(_curState)) {
                    if (_bufPrepParams->supportsTexturedLine()) {
                        curVertMem = _degenerateHelper->genDegenerate(
                                curVertMem, startCrds, endCrds, curZ
                        );
                    } else {
                        curVertMem = _degenerateHelper->genTexDegenerate(
                                curVertMem, startCrds, endCrds, curZ
                        );
                    }
                } else {
                    curVertMem = _degenerateHelper->genDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                }
            }
        }

        bool PolygonDrawer::isSemiTransparent() {
            if(_curStage == FILL_STAGE || !_haveOutline) {
                return _drawStateHelper->isFillStateTransparent(_curState);
            } else {
                return color_has_alpha(_stateHelper->getLastState().drawColor) ||
                       (_stateHelper->lineTextureUsed(_curState) && !_bufPrepParams->supportsTexturedLine());
            }
        }

        PolygonDrawer::~PolygonDrawer() {

        }

        void PolygonDrawer::setItemState(const ItemState &state) {
            _curState = state;
        }

        void PolygonDrawer::setItem(const QueueItem &item) {
            _curItem = item;
            _haveOutline = (!_stateHelper->isLineNull());
            _haveFill = _curItem.type == QueueItem::FILLPOLY;
            if(_haveOutline){
                createOutlineState();
                createOutlineTypeSize();
            }
            if(_haveFill){
                createFillState();
                createFillTypeSize();
            }
            if(_haveFill && _haveOutline){
                PolylinePolygon res = _polygonHelper->calcPolylinePolygon(
                        _curItem.data.drawPoly.numPoints, _curItem.data.drawPoly.points,
                        _stateHelper->getLastState().lineThickness, _stateHelper->lineTextureUsed(_outlineState),
                        _stateHelper->fillTextureUsed(_fillState)
                );
                _polygon = res.polygon;
                _polyline = res.polyline;
            } else if(_haveFill){
                _polygon = _polygonHelper->calcPolygon(
                        _curItem.data.drawPoly.numPoints, _curItem.data.drawPoly.points,
                        _stateHelper->fillTextureUsed(_fillState)
                );
            } else if(_haveOutline){
                _polyline = _polygonHelper->calcPolyline(
                        _curItem.data.drawPoly.numPoints, _curItem.data.drawPoly.points,
                        _stateHelper->getLastState().lineThickness, _stateHelper->lineTextureUsed(_outlineState)
                );
            }
        }

        PolygonDrawer::PolygonDrawer(DrawStateHelper *drawStateHelper, StateHelper *stateHelper,
                                     BufferPreparerParams *bufPrepParams, PropertyManager *propMan,
                                     SimplePrimHelper *simplePrimHelper, DegenerateHelper *degenerateHelper,
                                     TextureCoordsCalc *texCrdCalc, PolygonHelper *polygonHelper)
                :_drawStateHelper(drawStateHelper), _stateHelper(stateHelper), _bufPrepParams(bufPrepParams),
                 _propMan(propMan), _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper),
                 _texCrdCalc(texCrdCalc), _polygonHelper(polygonHelper) {

        }

        uint_fast8_t PolygonDrawer::getNumStages() {
            return static_cast<uint_fast8_t>((_haveFill && _haveOutline) ? 2 : 1);
        }

        void PolygonDrawer::setStage(uint_fast8_t stage) {
            _curStage = static_cast<PolyStage>(stage);
        }

        uint_fast32_t PolygonDrawer::getTotalSize() {
            uint_fast32_t size = 0;
            if(_haveOutline){
                size += (static_cast<uint_fast32_t>(_polyline.coords.size()) + 2) * _outlineTypeSize.sizeMult;
            }
            if(_haveFill){
                size += (static_cast<uint_fast32_t>(_polygon.coords.size()) + 2) * _fillTypeSize.sizeMult;
            }
            return size;
        }
    }
}

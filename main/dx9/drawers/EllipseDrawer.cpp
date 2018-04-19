#include <main/QueueItem.h>
#include <misc.h>
#include "VertexCreator.h"
#include "EllipseDrawer.h"

namespace directgraph{
    namespace dx9{
        void EllipseDrawer::getItemState(ItemState &state) {
            if(_curStage == FILL_STAGE || !_haveOutline){
                state = _fillState;
            } else {
                state = _outlineState;
            }
        }

        NumVertices EllipseDrawer::getNumVertices(bool isFirst) {
            NumVertices res;
            res.degenerate = (isFirst) ? 0 : 2;
            if(_curStage == FILL_STAGE || !_haveOutline){
                res.primitive = getNumFillVertices();
            } else {
                res.primitive = getNumOutlineVertices();
            }
            return res;
        }

        TypeSize EllipseDrawer::getTypeSize() {
            if(_curStage == FILL_STAGE || !_haveOutline){
                return _fillTypeSize;
            } else {
                return _outlineTypeSize;
            }
        }

        void EllipseDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &, float curZ) {
            if(_curStage == FILL_STAGE || !_haveOutline) {
                if(_haveOutline){
                    if (_stateHelper->fillTextureUsed(_curState)) {
                        if(_bufPrepParams->supportsTexturedBar()){
                            curVertMem = _simplePrimHelper->genFillCol2Triangles(curVertMem,
                                                                                 _ellipse.coords, _ellipse.texCoords,
                                                                                 curZ,
                                                                                 _stateHelper->getLastState().fillColor,
                                                                                 _stateHelper->getLastState().bgColor
                            );
                        } else {
                            curVertMem = _simplePrimHelper->genTexTriangles(curVertMem,
                                                                            _ellipse.coords, _ellipse.texCoords,
                                                                            curZ,
                                                                            _stateHelper->getLastState().fillColor
                            );
                        }
                    } else {
                        curVertMem = _simplePrimHelper->genTriangles(
                                curVertMem, _ellipse.coords, curZ, _stateHelper->getFillColor()
                        );
                    }
                } else {
                    if (
                            (_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsTexturedEllipse()) ||
                            (!_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsEllipse())
                            ) {
                        if (_stateHelper->fillTextureUsed(_curState)) {
                            curVertMem = _simplePrimHelper->genTexEllipseQuad(
                                    curVertMem,
                                    genFCoords(_curItem.data.ellipse.x, _curItem.data.ellipse.y),
                                    genFCoords(
                                            _curItem.data.ellipse.xradius - CORR_OFFSET,
                                            _curItem.data.ellipse.yradius - CORR_OFFSET
                                    ),
                                    curZ,
                                    _stateHelper->getLastState().fillColor,
                                    _stateHelper->getLastState().bgColor
                            );
                        } else {
                            curVertMem = _simplePrimHelper->genTexColorNoRHWQuad(
                                    curVertMem,
                                    genFCoords(_curItem.data.ellipse.x, _curItem.data.ellipse.y),
                                    genFCoords(
                                            _curItem.data.ellipse.xradius - CORR_OFFSET,
                                            _curItem.data.ellipse.yradius - CORR_OFFSET
                                    ),
                                    curZ,
                                    _stateHelper->getFillColor()
                            );
                        }
                    } else {
                        if (_stateHelper->fillTextureUsed(_curState)) {
                            curVertMem = _simplePrimHelper->genTexTriangles(
                                    curVertMem, _ellipse.coords, _ellipse.texCoords, curZ,
                                    _stateHelper->getLastState().fillColor
                            );
                        } else {
                            curVertMem = _simplePrimHelper->genTriangles(
                                    curVertMem, _ellipse.coords, curZ, _stateHelper->getFillColor()
                            );
                        }
                    }
                }
            } else {
                curVertMem = _simplePrimHelper->genTriangles(
                        curVertMem, _ellipseOutline.coords, curZ, _stateHelper->getLastState().drawColor
                );
            }
        }

        StartEndCoords EllipseDrawer::getStartEndCoords() {
            StartEndCoords res;
            if(_curStage == FILL_STAGE || !_haveOutline) {
                if (
                        !_haveOutline && (
                            (_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsTexturedEllipse()) ||
                            (!_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsEllipse())
                        )
                ) {
                    res.start = genFCoords(
                            _curItem.data.ellipse.x - _curItem.data.ellipse.xradius + CORR_OFFSET,
                            _curItem.data.ellipse.y - _curItem.data.ellipse.yradius + CORR_OFFSET
                    );
                    res.end = genFCoords(
                            _curItem.data.ellipse.x + _curItem.data.ellipse.xradius - CORR_OFFSET,
                            _curItem.data.ellipse.y + _curItem.data.ellipse.yradius - CORR_OFFSET
                    );
                } else {
                    res.start = _ellipse.coords.front();
                    res.end = _ellipse.coords.back();
                }
            } else {
                res.start = _ellipseOutline.coords.front();
                res.end = _ellipseOutline.coords.back();
            }
            return res;
        }

        void EllipseDrawer::genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds, float curZ) {
            if(_curStage == FILL_STAGE || !_haveOutline) {
                if(_haveOutline){
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
                    if (_stateHelper->fillTextureUsed(_curState)) {
                        if (_bufPrepParams->supportsTexturedEllipse()) {
                            curVertMem = _degenerateHelper->genTexEllipseDegenerate(
                                    curVertMem, startCrds, endCrds, curZ
                            );
                        } else {
                            curVertMem = _degenerateHelper->genTexDegenerate(
                                    curVertMem, startCrds, endCrds, curZ
                            );
                        }
                    } else {
                        if (_bufPrepParams->supportsEllipse()) {
                            curVertMem = _degenerateHelper->genTexColorNoRHWDegenerate(
                                    curVertMem, startCrds, endCrds,
                                    curZ
                            );
                        } else {
                            curVertMem = _degenerateHelper->genDegenerate(
                                    curVertMem, startCrds, endCrds, curZ
                            );
                        }
                    }
                }
            } else {
                curVertMem = _degenerateHelper->genDegenerate(
                        curVertMem, startCrds, endCrds, curZ
                );
            }
        }

        bool EllipseDrawer::isSemiTransparent() {
            if(_curStage == FILL_STAGE || !_haveOutline) {
                return _drawStateHelper->isFillStateTransparent(_curState);
            } else {
                return color_has_alpha(_stateHelper->getLastState().drawColor);
            }
        }

        EllipseDrawer::~EllipseDrawer() {

        }

        void EllipseDrawer::setItemState(const ItemState &state) {
            _curState = state;
        }

        void EllipseDrawer::setItem(const QueueItem &item) {
            _curItem = item;
            _haveOutline = (_stateHelper->getLastState().lineStyle != NULL_LINE);
            _haveFill = _curItem.type == QueueItem::FILLELLIPSE;
            if(_haveOutline){
                createOutlineState();
                createOutlineTypeSize();
            }
            if(_haveFill) {
                createFillState();
                createFillTypeSize();
            }
            if(_haveFill && !_haveOutline){
                _ellipse = _ellipseHelper->genEllipse(
                        genCoords(_curItem.data.ellipse.x, _curItem.data.ellipse.y),
                        genUCoords(
                                _curItem.data.ellipse.xradius,
                                _curItem.data.ellipse.yradius
                        ),
                        _curItem.data.ellipse.startAngle,
                        _curItem.data.ellipse.endAngle,
                        _stateHelper->fillTextureUsed(_fillState)
                );
            } else if(_haveOutline && !_haveFill){
                _ellipseOutline = _ellipseHelper->genOutline(
                        genCoords(_curItem.data.ellipse.x, _curItem.data.ellipse.y),
                        genUCoords(
                                _curItem.data.ellipse.xradius,
                                _curItem.data.ellipse.yradius
                        ),
                        _curItem.data.ellipse.startAngle,
                        _curItem.data.ellipse.endAngle,
                        _stateHelper->getLastState().lineThickness
                );
            } else {
                FullEllipse fEll = _ellipseHelper->genFullEllipse(
                        genCoords(_curItem.data.ellipse.x, _curItem.data.ellipse.y),
                        genUCoords(
                                _curItem.data.ellipse.xradius,
                                _curItem.data.ellipse.yradius
                        ),
                        _curItem.data.ellipse.startAngle,
                        _curItem.data.ellipse.endAngle,
                        _stateHelper->getLastState().lineThickness,
                        _stateHelper->fillTextureUsed(_fillState)
                );
                _ellipseOutline = fEll.outline;
                _ellipse = fEll.ellipse;
                if(_ellipse.coords.empty()){
                    _haveFill = false;
                }
            }
        }

        EllipseDrawer::EllipseDrawer(
                DrawStateHelper *drawStateHelper, StateHelper *stateHelper,
                BufferPreparerParams *bufPrepParams, PropertyManager *propMan,
                SimplePrimHelper *simplePrimHelper, EllipseHelper *ellipseHelper,
                DegenerateHelper *degenerateHelper
        ) : _drawStateHelper(drawStateHelper), _stateHelper(stateHelper),
            _bufPrepParams(bufPrepParams), _propMan(propMan),
            _simplePrimHelper(simplePrimHelper), _ellipseHelper(ellipseHelper),
            _degenerateHelper(degenerateHelper){
        }

        uint_fast8_t EllipseDrawer::getNumStages() {
            return static_cast<uint_fast8_t>((_haveFill && _haveOutline) ? 2 : 1);
        }

        void EllipseDrawer::setStage(uint_fast8_t stage) {
            _curStage = static_cast<PolyStage>(stage);
        }

        uint_fast32_t EllipseDrawer::getTotalSize() {
            uint_fast32_t size = 0;
            if(_haveFill){
                size += (getNumFillVertices() + 2) * _fillTypeSize.sizeMult;
            }
            if(_haveOutline){
                size += (getNumOutlineVertices() + 2) * _outlineTypeSize.sizeMult;
            }
            return size;
        }

        void EllipseDrawer::createFillState() {
            _fillState = _propMan->getNullState();
            _drawStateHelper->useFillTexture(
                    _fillState,
                    !(
                            (!_haveOutline && _bufPrepParams->supportsTexturedEllipse()) ||
                            (_haveOutline && _bufPrepParams->supportsTexturedBar())
                    ),
                    _bufPrepParams->needRecreateTexture()
            );
            if(!_haveOutline){
                if(_stateHelper->fillTextureUsed(_fillState)){
                    if(_bufPrepParams->supportsTexturedEllipse()){
                        _propMan->setProp(_fillState, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_ELLIPSE_SHADER);
                    }
                } else {
                    if (_bufPrepParams->supportsEllipse()) {
                        _propMan->setProp(_fillState, PropertyName::SHADER_TYPE, ShaderType::ELLIPSE_SHADER);
                    }
                }
            } else {
                if(_stateHelper->fillTextureUsed(_fillState)){
                    if(_bufPrepParams->supportsTexturedBar()){
                        _propMan->setProp(_fillState, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_BAR_SHADER);
                    }
                }
            }
        }

        void EllipseDrawer::createOutlineState() {
            _outlineState = _propMan->getNullState();
        }

        void EllipseDrawer::createOutlineTypeSize() {
            _outlineTypeSize.sizeMult = sizeof(ColorVertex);
            _outlineTypeSize.drawDataType = DrawDataType::COLOR_VERTEX;
        }

        void EllipseDrawer::createFillTypeSize() {
            if(_haveOutline){
                if (_stateHelper->fillTextureUsed(_fillState)) {
                    if(_bufPrepParams->supportsTexturedBar()){
                        _fillTypeSize.sizeMult = sizeof(Color2Vertex);
                        _fillTypeSize.drawDataType = DrawDataType::COLOR2_VERTEX;
                    } else {
                        _fillTypeSize.sizeMult = sizeof(TexturedColorVertex);
                        _fillTypeSize.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                    }
                } else {
                    _fillTypeSize.sizeMult = sizeof(ColorVertex);
                    _fillTypeSize.drawDataType = DrawDataType::COLOR_VERTEX;
                }
            } else {
                if (_stateHelper->fillTextureUsed(_fillState)) {
                    if (_bufPrepParams->supportsTexturedEllipse()) {
                        _fillTypeSize.sizeMult = sizeof(Color2Vertex);
                        _fillTypeSize.drawDataType = DrawDataType::TEXTURED_ELLIPSE_VERTEX;
                    } else {
                        _fillTypeSize.sizeMult = sizeof(TexturedColorVertex);
                        _fillTypeSize.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                    }
                } else {
                    if (_bufPrepParams->supportsEllipse()) {
                        _fillTypeSize.sizeMult = sizeof(TexturedColorVertexNoRHW);
                        _fillTypeSize.drawDataType = DrawDataType::ELLIPSE_VERTEX;
                    } else {
                        _fillTypeSize.sizeMult = sizeof(ColorVertex);
                        _fillTypeSize.drawDataType = DrawDataType::COLOR_VERTEX;
                    }
                }
            }
        }

        uint_fast32_t EllipseDrawer::getNumFillVertices() {
            uint_fast32_t res;
            if(_haveOutline){
                res = _ellipse.coords.size();
            } else {
                if ((_stateHelper->fillTextureUsed(_curState) && !_bufPrepParams->supportsTexturedEllipse()) ||
                    (!_stateHelper->fillTextureUsed(_curState) && !_bufPrepParams->supportsEllipse())) {
                    res = _ellipse.coords.size();
                } else {
                    res = VERTICES_IN_QUAD;
                }
            }
            return res;
        }

        uint_fast32_t EllipseDrawer::getNumOutlineVertices() {
            return _ellipseOutline.coords.size();
        }
    }
}
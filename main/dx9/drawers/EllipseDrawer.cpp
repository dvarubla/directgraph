#include "EllipseDrawer.h"

namespace directgraph{
    namespace dx9{
        void EllipseDrawer::getItemState(ItemState &_curState) {
            _drawStateHelper->disablePixelTexture(_curState);
            _drawStateHelper->useFillTexture(
                    _curState,
                    !_bufPrepParams->supportsTexturedEllipse(),
                    _bufPrepParams->needRecreateTexture()
            );
            if(_stateHelper->getLastState().lineStyle == NULL_LINE){
                if(_stateHelper->fillTextureUsed(_curState)){
                    if(_bufPrepParams->supportsTexturedEllipse()){
                        _propMan->setProp(_curState, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_ELLIPSE_SHADER);
                    } else {
                        _drawStateHelper->disableShader(_curState);
                    }
                } else {
                    if (_bufPrepParams->supportsEllipse()) {
                        _propMan->setProp(_curState, PropertyName::SHADER_TYPE, ShaderType::ELLIPSE_SHADER);
                    } else {
                        _drawStateHelper->disableShader(_curState);
                    }
                }
            }
        }

        NumVertices EllipseDrawer::getNumVertices(bool isFirst) {
            NumVertices res;
            res.degenerate = (isFirst) ? 0 : 2;
            if((_stateHelper->fillTextureUsed(_curState) && !_bufPrepParams->supportsTexturedEllipse()) ||
               (!_stateHelper->fillTextureUsed(_curState) && !_bufPrepParams->supportsEllipse())){
                res.primitive = _ellipseHelper->getNumEllipseVertices(
                        genUCoords(
                                _curItem.data.fillellipse.xradius,
                                _curItem.data.fillellipse.yradius
                        )
                );
                return res;
            }
            res.primitive = VERTICES_IN_QUAD;
            return res;
        }

        TypeSize EllipseDrawer::getTypeSize() {
            TypeSize res;
            if (_stateHelper->fillTextureUsed(_curState)) {
                if(_bufPrepParams->supportsTexturedEllipse()){
                    res.sizeMult = sizeof(Color2Vertex);
                    res.drawDataType = DrawDataType::TEXTURED_ELLIPSE_VERTEX;
                } else {
                    res.sizeMult = sizeof(TexturedColorVertex);
                    res.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                }
            } else {
                if (_bufPrepParams->supportsEllipse()) {
                    res.sizeMult = sizeof(TexturedColorVertexNoRHW);
                    res.drawDataType = DrawDataType::ELLIPSE_VERTEX;
                } else {
                    res.sizeMult = sizeof(ColorVertex);
                    res.drawDataType = DrawDataType::COLOR_VERTEX;
                }
            }
            return res;
        }

        void EllipseDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &, float curZ) {
            if (
                    (_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsTexturedEllipse()) ||
                    (!_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsEllipse())
            ) {
                if(_stateHelper->fillTextureUsed(_curState)){
                    curVertMem = _simplePrimHelper->genTexEllipseQuad(
                            curVertMem,
                            genCoords(_curItem.data.fillellipse.x, _curItem.data.fillellipse.y),
                            genUCoords(
                                    _curItem.data.fillellipse.xradius,
                                    _curItem.data.fillellipse.yradius
                            ),
                            curZ,
                            _stateHelper->getLastState().fillColor,
                            _stateHelper->getLastState().bgColor
                    );
                } else {
                    curVertMem = _simplePrimHelper->genEllipseQuad(
                            curVertMem,
                            genCoords(_curItem.data.fillellipse.x, _curItem.data.fillellipse.y),
                            genUCoords(
                                    _curItem.data.fillellipse.xradius,
                                    _curItem.data.fillellipse.yradius
                            ),
                            curZ,
                            _stateHelper->getFillColor()
                    );
                }
            } else {
                curVertMem = _ellipseHelper->genEllipse(
                        curVertMem,
                        genCoords(
                                _curItem.data.fillellipse.x,
                                _curItem.data.fillellipse.y
                        ),
                        genUCoords(
                                _curItem.data.fillellipse.xradius,
                                _curItem.data.fillellipse.yradius
                        ),
                        curZ,
                        _stateHelper->getLastState().fillColor,
                        _stateHelper->fillTextureUsed(_curState)
                );
            }
        }

        StartEndCoords EllipseDrawer::getStartEndCoords() {
            StartEndCoords res;
            if(
                    (_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsTexturedEllipse()) ||
                    (!_stateHelper->fillTextureUsed(_curState) && _bufPrepParams->supportsEllipse())
                    ){
                res.start = genCoords(
                        _curItem.data.fillellipse.x - _curItem.data.fillellipse.xradius,
                        _curItem.data.fillellipse.y - _curItem.data.fillellipse.yradius
                );
                res.end = genCoords(
                        _curItem.data.fillellipse.x + _curItem.data.fillellipse.xradius,
                        _curItem.data.fillellipse.y + _curItem.data.fillellipse.yradius
                );
            } else {
                res.start = res.end = genCoords(
                        _curItem.data.fillellipse.x,
                        _curItem.data.fillellipse.y - _curItem.data.fillellipse.yradius
                );
            }
            return res;
        }

        void EllipseDrawer::genDegenerates(void *&curVertMem, const Coords &startCrds, const Coords &endCrds, float curZ) {
            if(_stateHelper->fillTextureUsed(_curState)){
                if(_bufPrepParams->supportsTexturedEllipse()){
                    curVertMem = _degenerateHelper->genTexEllipseDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                } else {
                    curVertMem =  _degenerateHelper->genTexDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                }
            } else {
                if(_bufPrepParams->supportsEllipse()) {
                    curVertMem = _degenerateHelper->genEllipseDegenerate(
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

        bool EllipseDrawer::isSemiTransparent() {
            return _drawStateHelper->isFillStateTransparent(_curState);
        }

        EllipseDrawer::~EllipseDrawer() {

        }

        void EllipseDrawer::setItemState(const ItemState &state) {
            _curState = state;
        }

        void EllipseDrawer::setItem(const QueueItem &item) {
            _curItem = item;
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
    }
}
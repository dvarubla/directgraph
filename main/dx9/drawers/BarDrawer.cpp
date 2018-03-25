#include <VertexCreator.h>
#include "BarDrawer.h"

namespace directgraph{
    namespace dx9{
        void BarDrawer::getItemState(ItemState &state) {
            _drawStateHelper->useFillTexture(
                    state,
                    !_bufPrepParams->supportsTexturedBar(), _bufPrepParams->needRecreateTexture()
            );
            if(_bufPrepParams->supportsTexturedBar() && _stateHelper->fillTextureUsed(state)){
                _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_BAR_SHADER);
            }
        }

        NumVertices BarDrawer::getNumVertices(bool isFirst) {
            NumVertices res = {(isFirst) ? 0u : 2u, VERTICES_IN_QUAD};
            return res;
        }

        TypeSize BarDrawer::getTypeSize() {
            TypeSize res;
            if (_stateHelper->fillTextureUsed(_curState)) {
                if(_bufPrepParams->supportsTexturedBar()){
                    res.sizeMult = sizeof(Color2Vertex);
                    res.drawDataType = DrawDataType::COLOR2_VERTEX;
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

        void BarDrawer::processDrawItem(
                void *&curVertMem, uint_fast32_t &, float curZ
        ) {
            if (_stateHelper->fillTextureUsed(_curState)) {
                TextureCoords barCoords = _texCrdCalc->calcBarCoords(
                        genCoords(_curItem.data.bar.left, _curItem.data.bar.top),
                        genCoords(_curItem.data.bar.right, _curItem.data.bar.bottom)
                );
                if(_bufPrepParams->supportsTexturedBar()){
                    curVertMem = _simplePrimHelper->genFillCol2Quad(curVertMem,
                                                              genCoords(_curItem.data.bar.left, _curItem.data.bar.top),
                                                              genCoords(_curItem.data.bar.right, _curItem.data.bar.bottom),
                                                              curZ,
                                                              _stateHelper->getLastState().fillColor,
                                                              _stateHelper->getLastState().bgColor,
                                                              barCoords
                    );
                } else {
                    curVertMem = _simplePrimHelper->genTexColorQuad(curVertMem,
                        genCoords(_curItem.data.bar.left, _curItem.data.bar.top),
                        genCoords(_curItem.data.bar.right, _curItem.data.bar.bottom),
                        curZ,
                        _stateHelper->getLastState().fillColor,
                        barCoords, false
                    );
                }
            } else {
                curVertMem = _simplePrimHelper->genQuad(curVertMem,
                                                  genCoords(_curItem.data.bar.left, _curItem.data.bar.top),
                                                  genCoords(_curItem.data.bar.right, _curItem.data.bar.bottom),
                                                  curZ,
                                                  _stateHelper->getFillColor()

                );
            }
        }

        StartEndCoords BarDrawer::getStartEndCoords() {
            StartEndCoords res = {
                    genCoords(_curItem.data.bar.left, _curItem.data.bar.top),
                    genCoords(_curItem.data.bar.right, _curItem.data.bar.bottom)
            };
            return res;
        }

        void BarDrawer::genDegenerates(
                void *&curVertMem, const Coords &startCrds, const Coords &endCrds, float curZ
        ) {
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
        }

        bool BarDrawer::isSemiTransparent() {
            return _drawStateHelper->isFillStateTransparent(_curState);
        }

        BarDrawer::~BarDrawer() {

        }

        BarDrawer::BarDrawer(
                DrawStateHelper *drawStateHelper, StateHelper *stateHelper,
                BufferPreparerParams *bufPrepParams, PropertyManager *propMan,
                SimplePrimHelper *simplePrimHelper, DegenerateHelper *degenerateHelper,
                TextureCoordsCalc *texCrdCalc
        )
        :_drawStateHelper(drawStateHelper), _stateHelper(stateHelper), _bufPrepParams(bufPrepParams), _propMan(propMan),
         _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper), _texCrdCalc(texCrdCalc)
        {

        }

        void BarDrawer::setItemState(const ItemState &state) {
            _curState = state;
        }

        void BarDrawer::setItem(const QueueItem &item) {
            _curItem = item;
        }
    }
}
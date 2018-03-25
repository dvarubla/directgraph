#include <VertexCreator.h>
#include "RectangleDrawer.h"

namespace directgraph {
    namespace dx9 {
        void RectangleDrawer::getItemState(ItemState &state) {
            _drawStateHelper->useLineStyle(state, _bufPrepParams->needRecreateTexture() && !_bufPrepParams->supportsTexturedRectangle());
            if(_stateHelper->lineTextureUsed(state)){
                if(_bufPrepParams->supportsTexturedRectangle()){
                    _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_RECTANGLE_SHADER);
                }
            } else {
                if(_bufPrepParams->supportsRectangle()){
                    _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::RECTANGLE_SHADER);
                }
            }
        }

        NumVertices RectangleDrawer::getNumVertices(bool isFirst) {
            NumVertices res;
            res.degenerate = (isFirst) ? 0 : 2;
            if(
                    (_stateHelper->lineTextureUsed(_curState) && !_bufPrepParams->supportsTexturedRectangle()) || 
                    (!_stateHelper->lineTextureUsed(_curState) && !_bufPrepParams->supportsRectangle())
            ){
                res.primitive = VERTICES_IN_QUAD * 4;
            } else {
                res.primitive = VERTICES_IN_QUAD;
            }
            return res;
        }

        TypeSize RectangleDrawer::getTypeSize() {
            TypeSize res;
            if(_stateHelper->lineTextureUsed(_curState)) {
                res.sizeMult = sizeof(TexturedColorVertex);
                if(_bufPrepParams->supportsTexturedRectangle()){
                    res.drawDataType = DrawDataType::TEXTURED_RECTANGLE_VERTEX;
                } else {
                    res.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                }
            } else {
                if(_bufPrepParams->supportsRectangle()) {
                    res.sizeMult = sizeof(TexturedColorVertex);
                    res.drawDataType = DrawDataType::RECTANGLE_VERTEX;
                } else {
                    res.sizeMult = sizeof(ColorVertex);
                    res.drawDataType = DrawDataType::COLOR_VERTEX;
                }
            }
            return res;
        }

        void RectangleDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &, float curZ) {
            if(
                    (_stateHelper->lineTextureUsed(_curState) && _bufPrepParams->supportsTexturedRectangle()) ||
                    (!_stateHelper->lineTextureUsed(_curState) && _bufPrepParams->supportsRectangle())
            ){
                curVertMem = _simplePrimHelper->genTexRectangle(curVertMem,
                                                          genCoords(_curItem.data.rectangle.left,
                                                                    _curItem.data.rectangle.top),
                                                          genCoords(_curItem.data.rectangle.right,
                                                                    _curItem.data.rectangle.bottom),
                                                          _stateHelper->getLastState().lineThickness,
                                                          curZ,
                                                          _stateHelper->getLastState().drawColor
                );
            } else {
                curVertMem = _rectangleHelper->genRectangle(curVertMem,
                                                       genCoords(_curItem.data.rectangle.left,
                                                                 _curItem.data.rectangle.top),
                                                       genCoords(_curItem.data.rectangle.right,
                                                                 _curItem.data.rectangle.bottom),
                                                       _stateHelper->getLastState().lineThickness,
                                                       curZ,
                                                       _stateHelper->getLastState().drawColor,
                                                       _stateHelper->lineTextureUsed(_curState)
                );
            }
        }

        StartEndCoords RectangleDrawer::getStartEndCoords() {
            StartEndCoords res;
            if(
                    (_stateHelper->lineTextureUsed(_curState) && _bufPrepParams->supportsTexturedRectangle()) ||
                    (!_stateHelper->lineTextureUsed(_curState) && _bufPrepParams->supportsRectangle())
                    ){
                int_fast32_t thickness = _stateHelper->getLastState().lineThickness / 2;
                res.start = genCoords(_curItem.data.rectangle.left - thickness, _curItem.data.rectangle.top - thickness);
                res.end = genCoords(_curItem.data.rectangle.right + thickness, _curItem.data.rectangle.bottom + thickness);
            } else {
                res = _rectangleHelper->getCoords(
                        genCoords(_curItem.data.rectangle.left, _curItem.data.rectangle.top),
                        genCoords(_curItem.data.rectangle.right, _curItem.data.rectangle.bottom),
                        _stateHelper->getLastState().lineThickness
                );
            }
            return res;
        }

        void RectangleDrawer::genDegenerates(void *&curVertMem, const Coords &startCrds, const Coords &endCrds, float curZ) {
            if(_stateHelper->lineTextureUsed(_curState)) {
                if (_bufPrepParams->supportsTexturedRectangle()) {
                    curVertMem = _degenerateHelper->genShaderRectangleDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                } else {
                    curVertMem = _degenerateHelper->genTexDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                }
            } else {
                if (_bufPrepParams->supportsRectangle()) {
                    curVertMem = _degenerateHelper->genShaderRectangleDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                } else {
                    curVertMem = _degenerateHelper->genDegenerate(
                            curVertMem, startCrds, endCrds, curZ
                    );
                }
            }
        }

        bool RectangleDrawer::isSemiTransparent() {
            return color_has_alpha(_stateHelper->getLastState().drawColor) ||
                   (_stateHelper->lineTextureUsed(_curState) && !_bufPrepParams->supportsTexturedRectangle());
        }

        RectangleDrawer::RectangleDrawer(
                DrawStateHelper *drawStateHelper,
                StateHelper *stateHelper,
                BufferPreparerParams *bufPrepParams,
                PropertyManager *propMan,
                SimplePrimHelper *simplePrimHelper,
                DegenerateHelper *degenerateHelper,
                RectangleHelper *rectangleHelper
        ) : _drawStateHelper(drawStateHelper), _stateHelper(stateHelper), _bufPrepParams(bufPrepParams), _propMan(propMan),
            _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper), _rectangleHelper(rectangleHelper){

        }

        RectangleDrawer::~RectangleDrawer() {
        }

        void RectangleDrawer::setItemState(const ItemState &state) {
            _curState = state;
        }

        void RectangleDrawer::setItem(const QueueItem &item) {
            _curItem = item;
        }
    }
}

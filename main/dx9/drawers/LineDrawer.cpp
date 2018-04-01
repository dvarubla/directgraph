#include "LineDrawer.h"
#include <VertexCreator.h>
#include <main/QueueItem.h>
#include <main/util.h>

namespace directgraph{
    namespace dx9{
        void LineDrawer::getItemState(ItemState &state) {
            _drawStateHelper->useLineStyle(
                    state,
                    _bufPrepParams->needRecreateTexture()
            );
        }

        NumVertices LineDrawer::getNumVertices(bool isFirst) {
            NumVertices res = {(isFirst) ? 0u : 2u, VERTICES_IN_QUAD};
            return res;
        }

        TypeSize LineDrawer::getTypeSize() {
            TypeSize res;
            if (_stateHelper->lineTextureUsed(_curState)) {
                res.sizeMult = sizeof(TexturedColorVertex);
                res.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
            } else {
                res.sizeMult = sizeof(ColorVertex);
                res.drawDataType = DrawDataType::COLOR_VERTEX;
            }
            return res;
        }

        void LineDrawer::processDrawItem(
                void *&curVertMem, uint_fast32_t &, float curZ
        ) {
            if (_stateHelper->lineTextureUsed(_curState)) {
                TextureCoords texCrds = _texCrdCalc->addHalfPixel(
                        _texCrdCalc->calcLineCoords(
                                genFCoords(0, 0),
                                genFCoords(_lineHelper->getLen(), 0)
                        )
                );
                curVertMem = _simplePrimHelper->genTexColorQuad(curVertMem,
                                                        _lineHelper->getPoints(),
                                                        curZ,
                                                        _stateHelper->getLastState().drawColor,
                                                        texCrds,
                                                        false
                );
            } else {
                curVertMem = _simplePrimHelper->genQuad(curVertMem,
                                                        _lineHelper->getPoints(),
                                                        curZ,
                                                        _stateHelper->getLastState().drawColor

                );
            }
        }

        StartEndCoords LineDrawer::getStartEndCoords() {
            StartEndCoords res = {
                    _lineHelper->getPoints()[0],
                    _lineHelper->getPoints()[3]
            };
            return res;
        }

        void LineDrawer::genDegenerates(
                void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds, float curZ
        ) {
            if (_stateHelper->lineTextureUsed(_curState)) {
                curVertMem = _degenerateHelper->genTexDegenerate(
                        curVertMem, startCrds, endCrds, curZ
                );
            } else {
                curVertMem = _degenerateHelper->genDegenerate(
                        curVertMem, startCrds, endCrds, curZ
                );
            }
        }

        bool LineDrawer::isSemiTransparent() {
            return color_has_alpha(_stateHelper->getLastState().drawColor) || _stateHelper->lineTextureUsed(_curState);
        }

        LineDrawer::~LineDrawer() {

        }

        LineDrawer::LineDrawer(
                DrawStateHelper *drawStateHelper, StateHelper *stateHelper,
                BufferPreparerParams *bufPrepParams, PropertyManager *propMan,
                SimplePrimHelper *simplePrimHelper, DegenerateHelper *degenerateHelper,
                TextureCoordsCalc *texCrdCalc, LineHelper *lineHelper
        )
                :_drawStateHelper(drawStateHelper), _stateHelper(stateHelper), _bufPrepParams(bufPrepParams), _propMan(propMan),
                 _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper), _texCrdCalc(texCrdCalc), _lineHelper(lineHelper)
        {

        }

        void LineDrawer::setItemState(const ItemState &state) {
            _curState = state;
            _lineHelper->calcPoints(
                    _curItem.data.line.left, 
                    _curItem.data.line.top, 
                    _curItem.data.line.right, 
                    _curItem.data.line.bottom,
                    _stateHelper->getLastState().lineThickness
            );
        }

        void LineDrawer::setItem(const QueueItem &item) {
            _curItem = item;
        }
    }
}
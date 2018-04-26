#include <main/QueueItem.h>
#include <VertexCreator.h>
#include "Bar3DDrawer.h"

namespace directgraph{
    namespace dx9{
        Bar3DDrawer::Bar3DDrawer(DrawStateHelper *drawStateHelper, StateHelper *stateHelper,
                                 BufferPreparerParams *bufPrepParams, PropertyManager *propMan,
                                 SimplePrimHelper *simplePrimHelper, DegenerateHelper *degenerateHelper,
                                 Bar3DHelper *bar3DHelper,
                                 TextureCoordsCalc *texCrdCalc, BarDrawer *barDrawer, RectangleDrawer *rectDrawer)
        :_drawStateHelper(drawStateHelper), _stateHelper(stateHelper), _bufPrepParams(bufPrepParams), _propMan(propMan),
        _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper), _bar3DHelper(bar3DHelper),
         _texCrdCalc(texCrdCalc),
        _barDrawer(barDrawer), _rectDrawer(rectDrawer){

        }

        void Bar3DDrawer::getItemState(ItemState &state) {
            if(_curStage == BAR_STAGE){
                state = _barState;
            } else if(_curStage == RECT_STAGE){
                state = _rectState;
            } else {
                state = _sidesState;
            }
        }

        NumVertices Bar3DDrawer::getNumVertices(bool isFirst) {
            if(drawingSides()){
                NumVertices res = {(isFirst) ? 0u : 2u, getNumSidesVerts()};
                return res;
            } else {
                return _curDrawer->getNumVertices(isFirst);
            }
        }

        TypeSize Bar3DDrawer::getTypeSize() {
            if(drawingSides()){
                return _sidesTypeSize;
            } else {
                return _curDrawer->getTypeSize();
            }
        }

        void Bar3DDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &numVertices, float curZ) {
            if(drawingSides()){
                curVertMem = _bar3DHelper->genBar3D(
                        curVertMem,
                        genCoords(_curItem.data.rectangle.left,
                                  _curItem.data.rectangle.top),
                        genCoords(_curItem.data.rectangle.right,
                                  _curItem.data.rectangle.bottom),
                        static_cast<uint_fast32_t>(_curItem.data.bar3D.depth),
                        _stateHelper->getLastState().lineThickness,
                        curZ,
                        _stateHelper->getLastState().drawColor,
                        _stateHelper->lineTextureUsed(_curState),
                        _curItem.data.bar3D.haveTop,
                        _bufPrepParams->supportsTexturedLine()
                );
            } else {
                _curDrawer->processDrawItem(curVertMem, numVertices, curZ);
            }
        }

        StartEndCoords Bar3DDrawer::getStartEndCoords() {
            if(drawingSides()){
                return _bar3DHelper->getCoords(
                        genCoords(_curItem.data.bar3D.left, _curItem.data.bar3D.top),
                        genCoords(_curItem.data.bar3D.right, _curItem.data.bar3D.bottom),
                        _stateHelper->getLastState().lineThickness,
                        _curItem.data.bar3D.haveTop
                );
            } else {
                return _curDrawer->getStartEndCoords();
            }
        }

        void
        Bar3DDrawer::genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds, float curZ) {
            if(drawingSides()){
                if (_stateHelper->lineTextureUsed(_curState)) {
                    if(_bufPrepParams->supportsTexturedLine()) {
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
            } else {
                return _curDrawer->genDegenerates(curVertMem, startCrds, endCrds, curZ);
            }
        }

        bool Bar3DDrawer::isSemiTransparent() {
            if(drawingSides()){
                return color_has_alpha(_stateHelper->getLastState().drawColor) ||
                (_stateHelper->lineTextureUsed(_curState) && !_bufPrepParams->supportsTexturedLine());
            } else {
                return _curDrawer->isSemiTransparent();
            }
        }

        Bar3DDrawer::~Bar3DDrawer() {

        }

        void Bar3DDrawer::setItemState(const ItemState &state) {
            _curState = state;
        }

        void Bar3DDrawer::setItem(const QueueItem &item) {
            _curItem = item;
            _haveLine = (!_stateHelper->isLineNull());
            if(_haveLine){
                QueueItem rectItem = QueueItemCreator::create<QueueItem::RECTANGLE>(
                        _curItem.data.bar3D.left,
                        _curItem.data.bar3D.top,
                        _curItem.data.bar3D.right,
                        _curItem.data.bar3D.bottom
                );
                _rectDrawer->setItem(rectItem);
                _rectState = _propMan->getNullState();
                _rectDrawer->getItemState(_rectState);
                _rectDrawer->setItemState(_rectState);
                _haveSides = _curItem.data.bar3D.depth != 0;
                if(_haveSides){
                    createSidesState();
                    createSidesTypeSize();
                }
            } else {
                _haveSides = false;
            }
            int_fast32_t offset =
                    _stateHelper->getLastState().lineThickness / 2 +
                    _stateHelper->getLastState().lineThickness % 2;
            QueueItem barItem = QueueItemCreator::create<QueueItem::BAR>(
                    _curItem.data.bar3D.left + offset,
                    _curItem.data.bar3D.top + offset,
                    _curItem.data.bar3D.right - offset,
                    _curItem.data.bar3D.bottom - offset
            );
            _barDrawer->setItem(barItem);
            _barState = _propMan->getNullState();
            _barDrawer->getItemState(_barState);
            _barDrawer->setItemState(_barState);
        }

        uint_fast8_t Bar3DDrawer::getNumStages() {
            return static_cast<uint_fast8_t>((_haveSides) ? 3 : ((_haveLine) ? 2 : 1));
        }

        void Bar3DDrawer::setStage(uint_fast8_t stage) {
            _curStage = static_cast<Bar3DStage>(stage);
            if(_curStage == RECT_STAGE){
                _curDrawer = _rectDrawer;
            } else if(_curStage == BAR_STAGE){
                _curDrawer = _barDrawer;
            }
        }

        uint_fast32_t Bar3DDrawer::getTotalSize() {
            uint_fast32_t totalSize = 0;
            NumVertices barNumVerts = _barDrawer->getNumVertices(true);
            totalSize += (barNumVerts.primitive + barNumVerts.degenerate) * _barDrawer->getTypeSize().sizeMult;
            if(_haveLine){
                NumVertices rectNumVerts = _rectDrawer->getNumVertices(true);
                totalSize += (rectNumVerts.primitive + rectNumVerts.degenerate) * _rectDrawer->getTypeSize().sizeMult;
                if(_haveSides){
                    totalSize += (2 + getNumSidesVerts()) * _sidesTypeSize.sizeMult;
                }
            }
            return totalSize;
        }

        bool Bar3DDrawer::drawingSides() {
            return _curStage == SIDES_STAGE;
        }

        void Bar3DDrawer::createSidesTypeSize() {
            if (_stateHelper->lineTextureUsed(_sidesState)) {
                if(_bufPrepParams->supportsTexturedLine()) {
                    _sidesTypeSize.sizeMult = sizeof(ColorVertex);
                    _sidesTypeSize.drawDataType = DrawDataType::TEXTURED_LINE_VERTEX;
                } else {
                    _sidesTypeSize.sizeMult = sizeof(TexturedColorVertex);
                    _sidesTypeSize.drawDataType = DrawDataType::TEXTURED_COLOR_VERTEX;
                }
            } else {
                _sidesTypeSize.sizeMult = sizeof(ColorVertex);
                _sidesTypeSize.drawDataType = DrawDataType::COLOR_VERTEX;
            }
        }

        void Bar3DDrawer::createSidesState() {
            _sidesState = _propMan->getNullState();
            _drawStateHelper->useLineStyle(_sidesState, _bufPrepParams->needRecreateTexture() && !_bufPrepParams->supportsTexturedLine());
            if(_stateHelper->lineTextureUsed(_sidesState)){
                if(_bufPrepParams->supportsTexturedLine()){
                    _propMan->setProp(_sidesState, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_LINE_SHADER);
                }
            }
        }

        bool Bar3DDrawer::haveTop() {
            return _curItem.data.bar3D.haveTop;
        }

        uint_fast32_t Bar3DDrawer::getNumSidesVerts() {
            if(haveTop()){
                return 5 * VERTICES_IN_QUAD + 2;
            } else {
                return 3 * VERTICES_IN_QUAD;
            }
        }
    }
}
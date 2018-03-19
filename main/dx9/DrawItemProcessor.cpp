#include <main/QueueItem.h>
#include "DrawItemProcessor.h"
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        DrawItemProcessor::TypeSize DrawItemProcessor::getTypeSize(const QueueItem &item, const ItemState &state) {
            TypeSize res;
            switch(item.type){
                case QueueItem::RECTANGLE:
                    res.sizeMult = sizeof(ColorVertex);
                    res.drawDataType = DrawDataType::COLOR_VERTEX;
                    break;
                case QueueItem::PIXEL_CONTAINER:
                    res.sizeMult = sizeof(TexturedVertex);
                    res.drawDataType = DrawDataType::TEXTURED_VERTEX;
                    break;
                case QueueItem::BAR:
                    if (_stateHelper->fillTextureUsed(state)) {
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
                    break;
                case QueueItem::SINGLE_PIXEL: case QueueItem::CLEAR:
                    res.sizeMult = sizeof(ColorVertex);
                    res.drawDataType = DrawDataType::COLOR_VERTEX;
                    break;
                case QueueItem::FILLELLIPSE:
                    if (_stateHelper->fillTextureUsed(state)) {
                        if(_bufPrepParams->supportsTexturedEllipse()){
                            res.sizeMult = sizeof(TexturedColor2Vertex);
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
                    break;
                default: break;
            }
            return res;
        }

        void DrawItemProcessor::genDegenerates(
                const QueueItem &item, void *&curVertMem, const Coords &startCrds, const Coords &endCrds,
                const ItemState &state
        ){
            if (_stateHelper->fillTextureUsed(state)) {
                switch(item.type) {
                    case QueueItem::BAR: {
                        if(_bufPrepParams->supportsTexturedBar()){
                            curVertMem = _primCreator.genFillCol2Degenerate(
                                    curVertMem, startCrds, endCrds,
                                    _bufPrepParams->getMaxCoords(),
                                    _curZ
                            );
                        } else {
                            curVertMem = _primCreator.genFillDegenerate(
                                    curVertMem, startCrds, endCrds, _curZ
                            );
                        }
                    }
                        break;
                    case QueueItem::FILLELLIPSE: {
                        if(_bufPrepParams->supportsTexturedEllipse()){
                            curVertMem = _primCreator.genTexEllipseDegenerate(
                                    curVertMem, startCrds, endCrds, _curZ,
                                    _bufPrepParams->getMaxCoords()
                            );
                        } else {
                            curVertMem = _primCreator.genFillDegenerate(
                                    curVertMem, startCrds, endCrds, _curZ
                            );
                        }
                    }
                        break;
                    default:
                        break;
                }
            } else {
                switch(item.type){
                    case QueueItem::FILLELLIPSE: {
                        if (_bufPrepParams->supportsEllipse()) {
                            curVertMem = _primCreator.genEllipseDegenerate(
                                    curVertMem, startCrds, endCrds,
                                    _bufPrepParams->getMaxCoords(),
                                    _curZ
                            );
                        } else {
                            curVertMem = _primCreator.genDegenerate(
                                    curVertMem, startCrds, endCrds, _curZ
                            );
                        }
                    }
                        break;
                    case QueueItem::SINGLE_PIXEL:
                        curVertMem = _primCreator.genDegenerate(
                                curVertMem, startCrds, endCrds,
                                _curZ
                        );
                        break;
                    case QueueItem::RECTANGLE:
                        curVertMem = _primCreator.genDegenerate(
                                curVertMem, startCrds, endCrds, _curZ
                        );
                        break;
                    case QueueItem::BAR:
                        curVertMem = _primCreator.genDegenerate(
                                curVertMem, startCrds, endCrds, _curZ
                        );
                        break;
                    case QueueItem::CLEAR:
                        curVertMem = _primCreator.genDegenerate(
                                curVertMem, startCrds, endCrds, _curZ
                        );
                        break;
                }
            }
        }

        StartEndCoords DrawItemProcessor::getStartEndCoords(
                const QueueItem &item, const ItemState &state
        ) {
            StartEndCoords crds;
            switch(item.type){
                case QueueItem::SINGLE_PIXEL:
                    crds.start = genCoords(item.data.singlePixel.x, item.data.singlePixel.y);
                    crds.end = genCoords(item.data.singlePixel.x + 1, item.data.singlePixel.y + 1);
                    break;
                case QueueItem::CLEAR:
                    crds.start = genCoords(0, 0);
                    crds.end = genCoords(_bufPrepParams->getPxTextureCoords());
                    break;
                case QueueItem::BAR:
                    crds.start = genCoords(item.data.bar.left, item.data.bar.top);
                    crds.end = genCoords(item.data.bar.right, item.data.bar.bottom);
                    break;
                case QueueItem::PIXEL_CONTAINER: {
                    Rectangle coords = item.data.pixelContainer->getCoords();
                    crds.start = genCoords(coords.left, coords.top);
                    crds.end = genCoords(coords.right, coords.bottom);
                }
                    break;
                case QueueItem::FILLELLIPSE:
                    if(
                            _stateHelper->fillTextureUsed(state) && _bufPrepParams->supportsTexturedEllipse() ||
                            !_stateHelper->fillTextureUsed(state) && _bufPrepParams->supportsEllipse()
                            ){
                        crds.start = genCoords(
                                item.data.fillellipse.x - item.data.fillellipse.xradius,
                                item.data.fillellipse.y - item.data.fillellipse.yradius
                        );
                        crds.end = genCoords(
                                item.data.fillellipse.x + item.data.fillellipse.xradius,
                                item.data.fillellipse.y + item.data.fillellipse.yradius
                        );
                    } else {
                        crds.start = crds.end = genCoords(
                                item.data.fillellipse.x,
                                item.data.fillellipse.y - item.data.fillellipse.yradius
                        );
                    }
                    break;
                case QueueItem::RECTANGLE:
                    crds = _primCreator.getRectangleCoords(
                            genCoords(item.data.rectangle.left, item.data.rectangle.top),
                            genCoords(item.data.rectangle.right, item.data.rectangle.bottom),
                            _stateHelper->getLastState().lineThickness
                    );
            }
            return crds;
        }

        void DrawItemProcessor::processDrawItem(
                const QueueItem &item, void *&curVertMem,
                uint_fast32_t &,
                const ItemState &state
        ) {
            switch(item.type){
                case QueueItem::RECTANGLE:
                    curVertMem = _primCreator.genRectangle(curVertMem,
                                                          genCoords(item.data.rectangle.left, item.data.rectangle.top),
                                                          genCoords(item.data.rectangle.right, item.data.rectangle.bottom),
                                                          _stateHelper->getLastState().lineThickness,
                                                          _curZ,
                                                          _stateHelper->getLastState().drawColor
                    );
                    break;
                case QueueItem::SINGLE_PIXEL:
                    curVertMem = _primCreator.genQuad(curVertMem,
                                                      genCoords(item.data.singlePixel.x, item.data.singlePixel.y),
                                                      genCoords(item.data.singlePixel.x + 1, item.data.singlePixel.y + 1),
                                                      _curZ,
                                                      item.data.singlePixel.color
                    );
                    break;
                case QueueItem::CLEAR:
                    curVertMem = _primCreator.genQuad(curVertMem,
                                                      genCoords(0, 0),
                                                      genCoords(_bufPrepParams->getMaxCoords()),
                                                      _curZ,
                                                      0xFFFFFF
                    );
                    break;
                case QueueItem::FILLELLIPSE:
                    if (_stateHelper->fillTextureUsed(state)) {
                        if (_bufPrepParams->supportsTexturedEllipse()) {
                            curVertMem = _primCreator.genTexEllipseQuad(curVertMem,
                                                                        genCoords(item.data.fillellipse.x, item.data.fillellipse.y),
                                                                        genUCoords(
                                                                                item.data.fillellipse.xradius,
                                                                                item.data.fillellipse.yradius
                                                                        ),
                                                                        _curZ,
                                                                        _stateHelper->getLastState().fillColor,
                                                                        _stateHelper->getLastState().bgColor,
                                                                        _bufPrepParams->getMaxCoords()
                            );
                        } else {
                            curVertMem = _primCreator.genEllipse(curVertMem,
                                                                 genCoords(
                                                                         item.data.fillellipse.x,
                                                                         item.data.fillellipse.y
                                                                 ),
                                                                 genUCoords(
                                                                         item.data.fillellipse.xradius,
                                                                         item.data.fillellipse.yradius
                                                                 ),
                                                                 _curZ,
                                                                 _stateHelper->getLastState().fillColor,
                                                                 true
                            );
                        }
                    } else {
                        if (_bufPrepParams->supportsEllipse()) {
                            curVertMem = _primCreator.genEllipseQuad(curVertMem,
                                                                     genCoords(item.data.fillellipse.x, item.data.fillellipse.y),
                                                                     genUCoords(
                                                                             item.data.fillellipse.xradius,
                                                                             item.data.fillellipse.yradius
                                                                     ),
                                                                     _bufPrepParams->getMaxCoords(),
                                                                     _curZ,
                                                                     _stateHelper->getFillColor()
                            );
                        } else {
                            curVertMem = _primCreator.genEllipse(curVertMem,
                                                                 genCoords(
                                                                         item.data.fillellipse.x,
                                                                         item.data.fillellipse.y
                                                                 ),
                                                                 genUCoords(
                                                                         item.data.fillellipse.xradius,
                                                                         item.data.fillellipse.yradius
                                                                 ),
                                                                 _curZ,
                                                                 _stateHelper->getFillColor(),
                                                                 false
                            );
                        }
                    }
                    break;
                case QueueItem::BAR:
                    if (_stateHelper->fillTextureUsed(state)) {
                        if(_bufPrepParams->supportsTexturedBar()){
                            curVertMem = _primCreator.genFillCol2Quad(curVertMem,
                                                                      genCoords(item.data.bar.left, item.data.bar.top),
                                                                      genCoords(item.data.bar.right, item.data.bar.bottom),
                                                                      _curZ,
                                                                      _stateHelper->getLastState().fillColor,
                                                                      _stateHelper->getLastState().bgColor,
                                                                      _bufPrepParams->getMaxCoords()

                            );
                        } else {
                            curVertMem = _primCreator.genFillQuad(curVertMem,
                                                                  genCoords(item.data.bar.left, item.data.bar.top),
                                                                  genCoords(item.data.bar.right, item.data.bar.bottom),
                                                                  _curZ,
                                                                  _stateHelper->getLastState().fillColor
                            );
                        }
                    } else {
                        curVertMem = _primCreator.genQuad(curVertMem,
                                                          genCoords(item.data.bar.left, item.data.bar.top),
                                                          genCoords(item.data.bar.right, item.data.bar.bottom),
                                                          _curZ,
                                                          _stateHelper->getFillColor()

                        );
                    }
                    break;
                case QueueItem::PIXEL_CONTAINER: {
                    Rectangle coords = item.data.pixelContainer->getCoords();
                    curVertMem = _primCreator.genTexQuad(
                            curVertMem,
                            genCoords(coords.left, coords.top), genCoords(coords.right, coords.bottom),
                            _bufPrepParams->getPxTextureCoords(),
                            _curZ
                    );
                }
                    break;
                default:break;
            }
        }

        DrawItemProcessor::NumVertices DrawItemProcessor::getNumVertices(const QueueItem &item, bool isFirst) {
            NumVertices res;
            res.degenerate = (isFirst) ? 0 : 2;
            if(item.type == QueueItem::FILLELLIPSE && !_bufPrepParams->supportsEllipse()){
                res.primitive = _primCreator.getNumEllipseVertices(
                        genUCoords(
                                item.data.fillellipse.xradius,
                                item.data.fillellipse.yradius
                        )
                );
                return res;
            }
            if(item.type == QueueItem::RECTANGLE){
                res.primitive = VERTICES_IN_QUAD * 4;
                return res;
            }
            res.primitive = VERTICES_IN_QUAD;
            return res;
        }

        bool DrawItemProcessor::canCreateMoreItems() {
            return _itemNum != 0;
        }

        void DrawItemProcessor::nextItem() {
            _itemNum--;
            setCurZ();
        }

        void DrawItemProcessor::resetItemCount() {
            _itemNum = _bufPrepParams->getMaxDepthValues() - 1;
            setCurZ();
        }

        void DrawItemProcessor::setCurZ() {
            _curZ = _itemNum * _bufPrepParams->getDepthIncrement();
        }

        DrawItemProcessor::DrawItemProcessor(
                StateHelper *stateHelper, BufferPreparerParams *bufPrepParams
        ): _stateHelper(stateHelper), _bufPrepParams(bufPrepParams) {
            resetItemCount();
        }
    }
}

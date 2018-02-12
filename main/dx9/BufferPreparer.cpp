#include <main/WException.h>
#include <main/DPIHelper.h>
#include "Exception.h"
#include "BufferPreparer.h"
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        BufferPreparer::BufferPreparer(
                uint_fast32_t memSize,
                const BufferPreparer::DevDrawState *state,
                DPIHelper *helper,
                uint_fast32_t pxTextureWidth, uint_fast32_t pxTextureHeight,
                const GenDataVars &vars
        ): _memSize(memSize), _state(state), _helper(helper),
           _pixelTextureWidth(pxTextureWidth), _pixelTextureHeight(pxTextureHeight),
           _curGenDataVars(vars), _curUsedSize(0)
        {
            _vertMem = malloc(memSize);
            if (_vertMem == NULL) {
                THROW_EXC_CODE(
                        Exception,
                        CANT_ALLOC,
                        std::wstring(L"Can't allocate memory for vertices")
                );
            }
            _lastState = *_state;
        }

        void BufferPreparer::prepareBuffer(IQueueReader *reader) {
            uint_fast32_t size = reader->getSize();
            uint_fast32_t readIndex = 0;
            int_fast32_t prevX = 0, prevY = 0;
            bool isFirst = true;
            void *curVertMem = (static_cast<uint8_t*>(_vertMem) + _curUsedSize);

            DevDrawState tempState = *_state;
            if(tempState.userFillPattern != NULL){
                _patterns.push_back(tempState.userFillPattern);
            }
            for (readIndex = 0; readIndex < size; readIndex++) {
                QueueItem &item = reader->getAt(readIndex);
                if(item.type == QueueItem::SINGLE_PIXEL){
                    if(tempState.fillPattern != SOLID_FILL){
                        DrawOp op;
                        op.type = SET_FILL_PATTERN;
                        op.data.fillPattern = SOLID_FILL;
                        _drawOps.push_back(op);
                        tempState.fillPattern = SOLID_FILL;
                        isFirst = true;
                    }
                } else if(item.type == QueueItem::BAR){
                    if(tempState.userFillPattern != _lastState.userFillPattern && _lastState.fillPattern == USER_FILL){
                        DrawOp op;
                        op.type = SET_USER_FILL_PATTERN;
                        op.data.userFillPattern = _lastState.userFillPattern;
                        _drawOps.push_back(op);
                        tempState.userFillPattern = _lastState.userFillPattern;
                        isFirst = true;
                    }
                    if(tempState.fillPattern != _lastState.fillPattern){
                        DrawOp op;
                        op.type = SET_FILL_PATTERN;
                        op.data.fillPattern = _lastState.fillPattern;
                        _drawOps.push_back(op);
                        tempState.fillPattern = _lastState.fillPattern;
                        isFirst = true;
                    }
                    if(tempState.bgColor != _lastState.bgColor && tempState.fillPattern != SOLID_FILL){
                        DrawOp op;
                        op.type = SET_BG_COLOR;
                        op.data.bgColor = _lastState.bgColor;
                        _drawOps.push_back(op);
                        tempState.bgColor = _lastState.bgColor;
                        isFirst = true;
                    }
                } else if(item.type == QueueItem::CLEAR){
                    DrawOp op;
                    op.type = CLEAR;
                    _drawOps.push_back(op);
                } else if(item.type == QueueItem::PIXEL_CONTAINER){
                    DrawOp op;
                    if(tempState.fillPattern != SOLID_FILL){
                        op.type = SET_FILL_PATTERN;
                        op.data.fillPattern = SOLID_FILL;
                        _drawOps.push_back(op);
                        tempState.fillPattern = SOLID_FILL;
                    }
                    op.type = SET_PIXEL_TEXTURE;
                    op.data.pixelContainer = item.data.pixelContainer;
                    _drawOps.push_back(op);
                    isFirst = true;
                }
                if (
                        item.type == QueueItem::SINGLE_PIXEL ||
                        item.type == QueueItem::BAR ||
                        item.type == QueueItem::PIXEL_CONTAINER
                        ) {
                    uint_fast32_t curNumVertices;
                    uint_fast32_t sizeMult;
                    DrawDataType drawDataType;
                    bool useFillTexture = tempState.fillPattern != SOLID_FILL;
                    bool haveLastStride;
                    if(item.type == QueueItem::PIXEL_CONTAINER){
                        IPixelContainer *cont = item.data.pixelContainer;
                        haveLastStride = cont->getLastStride() != cont->getStride();
                        if(haveLastStride && cont->getHeight() != 1){
                            curNumVertices = VERTICES_IN_QUAD * 3 - VERTICES_TRIANGLES_DIFF;
                        } else {
                            curNumVertices = VERTICES_IN_QUAD;
                        }
                        sizeMult = sizeof(PrimitiveCreator::TexturedVertex);
                        drawDataType = TEXTURED_VERTEX;
                    } else {
                        curNumVertices =
                                (isFirst) ?
                                VERTICES_IN_QUAD :
                                VERTICES_IN_QUAD * 2 - VERTICES_TRIANGLES_DIFF
                                ;
                        if(useFillTexture){
                            sizeMult = sizeof(PrimitiveCreator::TexturedRectVertex);
                            drawDataType = TEXTURED_RECT_VERTEX;
                        } else {
                            sizeMult = sizeof(PrimitiveCreator::RectVertex);
                            drawDataType = RECT_VERTEX;
                        }
                    }
                    uint_fast32_t curUsedSize = static_cast<uint_fast32_t>(
                            static_cast<uint8_t*>(curVertMem) - static_cast<uint8_t*>(_vertMem)
                    );
                    uint_fast32_t newUsedSize = curUsedSize * sizeMult;
                    if (newUsedSize > _memSize) {
                        break;
                    }
                    if(isFirst){
                        DrawOp op;
                        op.type = ITEMS;
                        op.data.items.numItems = curNumVertices - VERTICES_TRIANGLES_DIFF;
                        op.data.items.offset = curUsedSize;
                        op.data.items.type = drawDataType;
                        _drawOps.push_back(op);
                    } else {
                        _drawOps.back().data.items.numItems += curNumVertices;
                    }
                    curUsedSize = newUsedSize;
                    if(item.type == QueueItem::PIXEL_CONTAINER){
                        IPixelContainer *cont = item.data.pixelContainer;
                        Rectangle firstCoords = cont->getFirstCoords();
                        Rectangle lastCoords = cont->getLastCoords();
                        firstCoords.right++;
                        firstCoords.bottom++;
                        lastCoords.right++;
                        lastCoords.bottom++;
                        bool nonZeroHeight = cont->getHeight() != 1 || !haveLastStride;
                        if (nonZeroHeight) {
                            curVertMem = _primCreator.genTexQuad(
                                    curVertMem,
                                    firstCoords.left, firstCoords.top, firstCoords.right, firstCoords.bottom,
                                    _pixelTextureWidth, _pixelTextureHeight
                            );
                        }
                        if (haveLastStride) {
                            if(nonZeroHeight) {
                                curVertMem = _primCreator.genTexDegenerate(
                                        curVertMem,
                                        firstCoords.right, firstCoords.bottom,
                                        lastCoords.left, lastCoords.top
                                );
                            }
                            curVertMem = _primCreator.genTexQuad(
                                    curVertMem,
                                    lastCoords.left, lastCoords.top, lastCoords.right, lastCoords.bottom,
                                    _pixelTextureWidth, _pixelTextureHeight
                            );
                        }
                    } else {
                        if (!isFirst) {
                            if (tempState.fillPattern != SOLID_FILL) {
                                curVertMem = _primCreator.genFillDegenerate(
                                        curVertMem,
                                        prevX, prevY,
                                        _helper->toPixelsX(item.data.bar.left),
                                        _helper->toPixelsY(item.data.bar.top)
                                );
                            } else {
                                curVertMem = _primCreator.genDegenerate(
                                        curVertMem,
                                        prevX, prevY,
                                        ((item.type == QueueItem::SINGLE_PIXEL) ?
                                         static_cast<int_fast32_t>(item.data.singlePixel.x) :
                                         _helper->toPixelsX(item.data.bar.left)),
                                        ((item.type == QueueItem::SINGLE_PIXEL) ?
                                         static_cast<int_fast32_t>(item.data.singlePixel.y) :
                                         _helper->toPixelsY(item.data.bar.top))
                                );
                            }
                        }
                        if (item.type == QueueItem::SINGLE_PIXEL) {
                            curVertMem = _primCreator.genQuad(curVertMem,
                                                              item.data.singlePixel.x,
                                                              item.data.singlePixel.y,
                                                              item.data.singlePixel.x + 1,
                                                              item.data.singlePixel.y + 1,
                                                              item.data.singlePixel.color
                            );
                            prevX = item.data.singlePixel.x + 1;
                            prevY = item.data.singlePixel.y + 1;
                        } else {
                            if (useFillTexture) {
                                curVertMem = _primCreator.genFillQuad(curVertMem,
                                                                      _helper->toPixelsX(item.data.bar.left),
                                                                      _helper->toPixelsY(item.data.bar.top),
                                                                      _helper->toPixelsX(item.data.bar.right),
                                                                      _helper->toPixelsY(item.data.bar.bottom),
                                                                      _curGenDataVars.fillColor
                                );
                            } else {
                                curVertMem = _primCreator.genQuad(curVertMem,
                                                                  _helper->toPixelsX(item.data.bar.left),
                                                                  _helper->toPixelsY(item.data.bar.top),
                                                                  _helper->toPixelsX(item.data.bar.right),
                                                                  _helper->toPixelsY(item.data.bar.bottom),
                                                                  ((_curGenDataVars.fillStyle == SOLID_FILL) ?
                                                                   _curGenDataVars.fillColor :
                                                                   _curGenDataVars.bgColor)
                                );
                            }
                            prevX = _helper->toPixelsX(item.data.bar.right);
                            prevY = _helper->toPixelsY(item.data.bar.bottom);
                        }
                    }
                    isFirst = (item.type == QueueItem::PIXEL_CONTAINER);
                } else if (item.type == QueueItem::SETFILLSTYLE) {
                    _curGenDataVars.fillColor = item.data.setfillstyle.color;
                    if(item.data.setfillstyle.fillStyle == EMPTY_FILL){
                        _curGenDataVars.fillStyle = EMPTY_FILL;
                        _lastState.fillPattern = SOLID_FILL;
                    } else {
                        _curGenDataVars.fillStyle = SOLID_FILL;
                        _lastState.fillPattern = item.data.setfillstyle.fillStyle;
                    }
                } else if (item.type == QueueItem::SETFILLPATTERN) {
                    _curGenDataVars.fillColor = item.data.setfillpattern.color;
                    _lastState.fillPattern = USER_FILL;
                    _curGenDataVars.fillStyle = SOLID_FILL;
                    _lastState.userFillPattern = item.data.setfillpattern.fillPattern;
                    _patterns.push_back(_lastState.userFillPattern);
                } else if (item.type == QueueItem::BGCOLOR){
                    _lastState.bgColor = item.data.bgColor;
                    _curGenDataVars.bgColor = item.data.bgColor;
                }
            }
            reader->endReading(readIndex);
            _curUsedSize = static_cast<uint_fast32_t>(static_cast<uint8_t*>(curVertMem) - static_cast<uint8_t*>(_vertMem));
        }

        void *BufferPreparer::getBuffer() {
            return _vertMem;
        }

        void BufferPreparer::clear() {
            if(!_patterns.empty()){
                _patterns.pop_back();
                for(CharPVector::iterator it = _patterns.begin(); it != _patterns.end(); ++it){
                    delete [] *it;
                }
            }
            _patterns.clear();
            _drawOps.clear();
            _curUsedSize = 0;
        }

        BufferPreparer::DrawOpVector::iterator
        BufferPreparer::drawOpsBegin() {
            return _drawOps.begin();
        }

        BufferPreparer::DrawOpVector::iterator
        BufferPreparer::drawOpsEnd() {
            return _drawOps.end();
        }

        bool BufferPreparer::isEmpty() {
            return _drawOps.empty();
        }

        uint_fast32_t BufferPreparer::getUsedSize() {
            return _curUsedSize;
        }

        BufferPreparer::~BufferPreparer() {
            free(_vertMem);
        }
    }
}
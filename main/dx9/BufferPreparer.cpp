#include <main/WException.h>
#include <main/DPIHelper.h>
#include <main/QueueItem.h>
#include "Exception.h"
#include "BufferPreparer.h"
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::CLEAR>() {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::CLEAR;
            return op;
        }

        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::REMOVE_TEXTURE>() {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::REMOVE_TEXTURE;
            return op;
        }

        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::SET_FILL_PATTERN>(uint_fast32_t fillPattern) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::SET_FILL_PATTERN;
            op.data.fillPattern = static_cast<uint8_t>(fillPattern);
            return op;
        }

        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::SET_FILL_PATTERN_COLOR>(
                uint_fast32_t fillPattern, uint_fast32_t bgColor
        ) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::SET_FILL_PATTERN_COLOR;
            op.data.fillPatternColor.fillPattern = static_cast<uint8_t>(fillPattern);
            op.data.fillPatternColor.bgColor = bgColor;
            return op;
        }

        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::SET_TEX_BG_COLOR>(uint32_t bgColor) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::SET_TEX_BG_COLOR;
            op.data.bgColor = bgColor;
            return op;
        }

        template<>
        BufferPreparer::DrawOp
        DrawOpCreator::create<BufferPreparer::ITEMS>(
                uint32_t offset, uint32_t numItems, BufferPreparer::DrawDataType type
        ) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::ITEMS;
            op.data.items.offset = offset;
            op.data.items.numItems = numItems;
            op.data.items.type = type;
            return op;
        }

        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::SET_PIXEL_TEXTURE>(
                IPixelContainer *container
        ) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::SET_PIXEL_TEXTURE;
            op.data.pixelContainer = container;
            return op;
        }

        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::SET_USER_FILL_PATTERN>(char *userFillPattern) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::SET_USER_FILL_PATTERN;
            op.data.userFillPattern = userFillPattern;
            return op;
        }

        BufferPreparer::BufferPreparer(
                uint_fast32_t memSize, const DevDrawState &state, DPIHelper *helper,
                BufferPreparerParams *bufPrepParams,
                const GenDataVars &vars
        ): _bufPrepParams(bufPrepParams), _memSize(memSize), _helper(helper),
           _curGenDataVars(vars), _curUsedSize(0), _lastOffset(0), _canReadMore(true)
        {
            _vertMem = malloc(memSize);
            if (_vertMem == NULL) {
                THROW_EXC_CODE(
                        Exception,
                        CANT_ALLOC,
                        std::wstring(L"Can't allocate memory for vertices")
                );
            }
            _lastState = state;
            _curState = state;
            setInitialSettings();
        }

        void BufferPreparer::setInitialSettings() {
            _curSettings.shaderType = NO_SHADER;
            _curSettings.bgColorSet = false;
            _curSettings.textureSet = false;
            _curSettings.fillPatternRecreated = true;
        }

        void BufferPreparer::useFillTexture(bool useBgColor) {
            if(_curState.userFillPattern != _lastState.userFillPattern && _lastState.fillPattern == USER_FILL) {
                _drawOps.push_back(DrawOpCreator::create<SET_USER_FILL_PATTERN>(_lastState.userFillPattern));
                _curState.userFillPattern = _lastState.userFillPattern;
                _isFirst = true;
            }

            bool patternChanged = false;

            if(_curState.fillPattern != _lastState.fillPattern) {
                _curState.fillPattern = _lastState.fillPattern;
                patternChanged = true;
            }
            if(!_curSettings.textureSet){
                _curSettings.textureSet = true;
                patternChanged = true;
            }
            if(_bufPrepParams->needRecreateTexture() && !_curSettings.fillPatternRecreated && useBgColor){
                patternChanged = true;
            }

            if(useBgColor){
                if(
                        (!_curSettings.bgColorSet || _curState.bgColor != _lastState.bgColor) &&
                        _curState.fillPattern != SOLID_FILL
                ){
                    _curSettings.bgColorSet = true;
                    _curState.bgColor = _lastState.bgColor;
                    if (_bufPrepParams->needRecreateTexture()) {
                        patternChanged = true;
                    } else {
                        _drawOps.push_back(DrawOpCreator::create<SET_TEX_BG_COLOR>(_lastState.bgColor));
                        _isFirst = true;
                    }
                }
            }

            if (_curState.fillPattern == SOLID_FILL) {
                if(patternChanged || _curState.textureState != NO_TEXTURE) {
                    _curState.textureState = NO_TEXTURE;
                    _drawOps.push_back(DrawOpCreator::create<REMOVE_TEXTURE>());
                    _isFirst = true;
                }
            } else {
                if(patternChanged || _curState.textureState != FILL_TEXTURE) {
                    _curState.textureState = FILL_TEXTURE;
                    if(_bufPrepParams->needRecreateTexture()){
                        _drawOps.push_back(DrawOpCreator::create<SET_FILL_PATTERN_COLOR>(
                                _curState.fillPattern, _curState.bgColor
                        ));
                    } else {
                        _drawOps.push_back(DrawOpCreator::create<SET_FILL_PATTERN>(_curState.fillPattern));
                    }
                    _curSettings.fillPatternRecreated = useBgColor;
                    _isFirst = true;
                }
            }
        }

        void BufferPreparer::disableTexture() {
            if(_curState.textureState != NO_TEXTURE){
                _curState.textureState = NO_TEXTURE;
                _drawOps.push_back(DrawOpCreator::create<REMOVE_TEXTURE>());
                _isFirst = true;
            }
        }

        void BufferPreparer::disableShader() {
            if(_curSettings.shaderType != NO_SHADER){
                _curSettings.shaderType = NO_SHADER;
                _isFirst = true;
            }
        }

        void BufferPreparer::processStateChanges(const QueueItem &item) {
            switch(item.type){
                case QueueItem::BAR:
                    disableShader();
                    useFillTexture(!_bufPrepParams->supportsTexturedBar());
                    break;
                case QueueItem::SINGLE_PIXEL:
                    disableShader();
                    disableTexture();
                    break;
                case QueueItem::PIXEL_CONTAINER:
                    disableShader();
                    disableTexture();
                    _drawOps.push_back(DrawOpCreator::create<SET_PIXEL_TEXTURE>(item.data.pixelContainer));
                    _isFirst = true;
                    break;
                case QueueItem::FILLELLIPSE:
                    useFillTexture(true);
                    if(_lastState.lineStyle == NULL_LINE){
                        if(_curState.textureState == LINE_TEXTURE && _curState.lineStyle != NULL_LINE) {
                            _curState.textureState = NO_TEXTURE;
                            _curState.lineStyle = NULL_LINE;
                            _drawOps.push_back(DrawOpCreator::create<REMOVE_TEXTURE>());
                            _isFirst = true;
                        }
                        if(_bufPrepParams->supportsEllipse()){
                            if(_curSettings.shaderType != ELLIPSE_SHADER) {
                                _isFirst = true;
                                _curSettings.shaderType = ELLIPSE_SHADER;
                            }
                        } else {
                            disableShader();
                        }
                    }
                    break;
                case QueueItem::CLEAR:
                    _drawOps.push_back(DrawOpCreator::create<CLEAR>());
                    break;
                default: break;
            }
        }

        BufferPreparer::TypeSize BufferPreparer::getTypeSize(const QueueItem &item) {
            TypeSize res;
            switch(item.type){
                case QueueItem::PIXEL_CONTAINER:
                    res.sizeMult = sizeof(TexturedVertex);
                    res.drawDataType = TEXTURED_VERTEX;
                    break;
                case QueueItem::BAR:
                    if (_curState.fillPattern != SOLID_FILL) {
                        if(_bufPrepParams->supportsTexturedBar()){
                            res.sizeMult = sizeof(TexturedColorVertex);
                            res.drawDataType = COLOR2_VERTEX;
                        } else {
                            res.sizeMult = sizeof(TexturedColorVertex);
                            res.drawDataType = TEXTURED_COLOR_VERTEX;
                        }
                    } else {
                        res.sizeMult = sizeof(ColorVertex);
                        res.drawDataType = COLOR_VERTEX;
                    }
                    break;
                case QueueItem::SINGLE_PIXEL:
                    res.sizeMult = sizeof(ColorVertex);
                    res.drawDataType = COLOR_VERTEX;
                    break;
                case QueueItem::FILLELLIPSE:
                    if(_bufPrepParams->supportsEllipse()) {
                        res.sizeMult = sizeof(ColorVertex);
                        res.drawDataType = ELLIPSE_VERTEX;
                    } else {
                        res.sizeMult = sizeof(ColorVertex);
                        res.drawDataType = COLOR_VERTEX;
                    }
                    break;
                default: break;
            }
            return res;
        }

        void BufferPreparer::processDrawItem(
                const QueueItem &item, void *&curVertMem, int_fast32_t &prevX, int_fast32_t &prevY
        ) {
            if (!_isFirst) {
                if (_curState.fillPattern != SOLID_FILL) {
                    switch(item.type) {
                            case QueueItem::BAR: {
                            if(_bufPrepParams->supportsTexturedBar()){
                                curVertMem = _primCreator.genFillCol2Degenerate(
                                        curVertMem,
                                        prevX, prevY,
                                        _helper->toPixelsX(item.data.bar.left),
                                        _helper->toPixelsY(item.data.bar.top),
                                        _bufPrepParams->getWidth(), _bufPrepParams->getHeight()
                                );
                            } else {
                                curVertMem = _primCreator.genFillDegenerate(
                                        curVertMem,
                                        prevX, prevY,
                                        _helper->toPixelsX(item.data.bar.left),
                                        _helper->toPixelsY(item.data.bar.top)
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
                                        curVertMem,
                                        prevX, prevY,
                                        _helper->toPixelsX(item.data.fillellipse.x - item.data.fillellipse.xradius),
                                        _helper->toPixelsY(item.data.fillellipse.y - item.data.fillellipse.yradius),
                                        _bufPrepParams->getWidth(), _bufPrepParams->getHeight()
                                );
                            } else {
                                curVertMem = _primCreator.genDegenerate(
                                        curVertMem,
                                        prevX, prevY,
                                        _helper->toPixelsX(item.data.fillellipse.x),
                                        _helper->toPixelsY(item.data.fillellipse.y - item.data.fillellipse.yradius)
                                );
                            }
                        }
                            break;
                        default:
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
                            break;
                    }
                }
            }
            switch(item.type){
                case QueueItem::SINGLE_PIXEL:
                    curVertMem = _primCreator.genQuad(curVertMem,
                                                      item.data.singlePixel.x,
                                                      item.data.singlePixel.y,
                                                      item.data.singlePixel.x + 1,
                                                      item.data.singlePixel.y + 1,
                                                      item.data.singlePixel.color
                    );
                    prevX = item.data.singlePixel.x + 1;
                    prevY = item.data.singlePixel.y + 1;
                    break;
                case QueueItem::FILLELLIPSE:
                    if(_bufPrepParams->supportsEllipse()) {
                        curVertMem = _primCreator.genEllipseQuad(curVertMem,
                                                                 _helper->toPixelsX(item.data.fillellipse.x),
                                                                 _helper->toPixelsY(item.data.fillellipse.y),
                                                                 _helper->toPixelsX(item.data.fillellipse.xradius),
                                                                 _helper->toPixelsY(item.data.fillellipse.yradius),
                                                                 _bufPrepParams->getWidth(), _bufPrepParams->getHeight(),
                                                                 _curGenDataVars.fillColor
                        );
                        prevX = _helper->toPixelsX(item.data.fillellipse.x + item.data.fillellipse.xradius);
                        prevY = _helper->toPixelsY(item.data.fillellipse.y + item.data.fillellipse.yradius);
                    } else {
                        curVertMem = _primCreator.genEllipse(curVertMem,
                                                             _helper->toPixelsX(item.data.fillellipse.x),
                                                             _helper->toPixelsY(item.data.fillellipse.y),
                                                             static_cast<uint_fast32_t>(_helper->toPixelsX(item.data.fillellipse.xradius)),
                                                             static_cast<uint_fast32_t>(_helper->toPixelsY(item.data.fillellipse.yradius)),
                                                             _curGenDataVars.fillColor
                        );
                        prevX = _helper->toPixelsX(item.data.fillellipse.x);
                        prevY = _helper->toPixelsY(item.data.fillellipse.y - item.data.fillellipse.yradius);
                    }
                    break;
                case QueueItem::BAR:
                    if (_curState.fillPattern != SOLID_FILL) {
                        if(_bufPrepParams->supportsTexturedBar()){
                            curVertMem = _primCreator.genFillCol2Quad(curVertMem,
                                                                  _helper->toPixelsX(item.data.bar.left),
                                                                  _helper->toPixelsY(item.data.bar.top),
                                                                  _helper->toPixelsX(item.data.bar.right),
                                                                  _helper->toPixelsY(item.data.bar.bottom),
                                                                  _curGenDataVars.fillColor, _curGenDataVars.bgColor,
                                                                  _bufPrepParams->getWidth(), _bufPrepParams->getHeight()

                            );
                        } else {
                            curVertMem = _primCreator.genFillQuad(curVertMem,
                                                                  _helper->toPixelsX(item.data.bar.left),
                                                                  _helper->toPixelsY(item.data.bar.top),
                                                                  _helper->toPixelsX(item.data.bar.right),
                                                                  _helper->toPixelsY(item.data.bar.bottom),
                                                                  _curGenDataVars.fillColor
                            );
                        }
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
                    break;
                case QueueItem::PIXEL_CONTAINER: {
                    Rectangle coords = item.data.pixelContainer->getCoords();
                    curVertMem = _primCreator.genTexQuad(
                            curVertMem,
                            coords.left, coords.top, coords.right, coords.bottom,
                            _bufPrepParams->getPxTextureWidth(), _bufPrepParams->getPxTextureHeight()
                    );
                }
                    break;
                default:break;
            }
            _isFirst = (item.type == QueueItem::PIXEL_CONTAINER);
        }

        void BufferPreparer::prepareBuffer(IQueueReader *reader, uint_fast32_t offset, uint_fast32_t maxSize) {
            uint_fast32_t size = reader->getSize();
            uint_fast32_t readIndex = 0;
            int_fast32_t prevX = 0, prevY = 0;
            _isFirst = true;
            void *curVertMem = (static_cast<uint8_t*>(_vertMem) + _curUsedSize);
            if(_patterns.empty() && _curState.userFillPattern != NULL){
                _patterns.push_back(_curState.userFillPattern);
            }
            for (readIndex = offset; readIndex < size; readIndex++) {
                QueueItem &item = reader->getAt(readIndex);
                processStateChanges(item);
                if (
                        item.type == QueueItem::SINGLE_PIXEL ||
                        item.type == QueueItem::BAR ||
                        item.type == QueueItem::PIXEL_CONTAINER ||
                        item.type == QueueItem::FILLELLIPSE
                ) {
                    TypeSize ts = getTypeSize(item);
                    uint_fast32_t curNumVertices;
                    if(item.type == QueueItem::FILLELLIPSE && !_bufPrepParams->supportsEllipse()){
                        curNumVertices = _primCreator.getNumEllipseVertices(
                                static_cast<uint_fast32_t>(_helper->toPixelsX(item.data.fillellipse.xradius)),
                                static_cast<uint_fast32_t>(_helper->toPixelsY(item.data.fillellipse.yradius))
                        );
                        if(!_isFirst){
                            curNumVertices += 2;
                        }
                    } else {
                        curNumVertices =
                                (_isFirst) ?
                                VERTICES_IN_QUAD :
                                VERTICES_IN_QUAD + 2
                        ;
                    }
                    uint_fast32_t curUsedSize = static_cast<uint_fast32_t>(
                            static_cast<uint8_t*>(curVertMem) - static_cast<uint8_t*>(_vertMem)
                    );
                    uint_fast32_t newUsedSize = curUsedSize + curNumVertices * ts.sizeMult;
                    if (newUsedSize > maxSize) {
                        _canReadMore = false;
                        break;
                    }
                    if(_isFirst){
                        _drawOps.push_back(DrawOpCreator::create<ITEMS>(
                                curUsedSize, curNumVertices - VERTICES_TRIANGLES_DIFF, ts.drawDataType
                        ));
                    } else {
                        _drawOps.back().data.items.numItems += curNumVertices;
                    }
                    processDrawItem(item, curVertMem, prevX, prevY);
                } else {
                    switch (item.type){
                        case QueueItem::BGCOLOR:
                            _lastState.bgColor = item.data.bgColor;
                            _curGenDataVars.bgColor = item.data.bgColor;
                            break;
                        case QueueItem::SETFILLSTYLE:
                            _curGenDataVars.fillColor = item.data.setfillstyle.color;
                            if(item.data.setfillstyle.fillStyle == EMPTY_FILL){
                                _curGenDataVars.fillStyle = EMPTY_FILL;
                                _lastState.fillPattern = SOLID_FILL;
                            } else {
                                _curGenDataVars.fillStyle = SOLID_FILL;
                                _lastState.fillPattern = item.data.setfillstyle.fillStyle;
                            }
                            break;
                        case QueueItem::SETFILLPATTERN:
                            _curGenDataVars.fillColor = item.data.setfillpattern.color;
                            _lastState.fillPattern = USER_FILL;
                            _curGenDataVars.fillStyle = SOLID_FILL;
                            _lastState.userFillPattern = item.data.setfillpattern.fillPattern;
                            _patterns.push_back(_lastState.userFillPattern);
                            break;
                        case QueueItem::SETLINESTYLE:
                            _lastState.lineStyle = item.data.setlinestyle.linestyle;
                            break;
                        default:
                            break;
                    }
                }
            }
            _curUsedSize = static_cast<uint_fast32_t>(static_cast<uint8_t*>(curVertMem) - static_cast<uint8_t*>(_vertMem));
            _lastOffset = readIndex;
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
            _canReadMore = true;
            setInitialSettings();
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

        bool BufferPreparer::isFull() {
            return !_canReadMore;
        }

        uint_fast32_t BufferPreparer::getLastOffset() {
            return _lastOffset;
        }

        void BufferPreparer::resetOffset() {
            _lastOffset = 0;
        }
    }
}
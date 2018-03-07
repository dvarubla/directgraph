#include <main/WException.h>
#include <main/DPIHelper.h>
#include <main/QueueItem.h>
#include "Exception.h"
#include "BufferPreparer.h"
#include "PrimitiveCreator.h"

namespace directgraph{
    namespace dx9{
        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::REMOVE_PIXEL_TEXTURE>() {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::REMOVE_PIXEL_TEXTURE;
            return op;
        }

        template<>
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::REMOVE_PATTERN_TEXTURE>() {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::REMOVE_PATTERN_TEXTURE;
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
                uint_fast32_t memSize, DPIHelper *helper,
                const CommonProps &props,
                BufferPreparerParams *bufPrepParams
        ): _bufPrepParams(bufPrepParams), _memSize(memSize), _stateHelper(&_propMan, props), _helper(helper),
           _isFirst(true), _curUsedSize(0), _lastOffset(0), _canReadMore(true)
        {
            _vertMem = malloc(memSize);
            if (_vertMem == NULL) {
                THROW_EXC_CODE(
                        Exception,
                        CANT_ALLOC,
                        std::wstring(L"Can't allocate memory for vertices")
                );
            }
        }

        void BufferPreparer::useFillTexture(ItemState &state, bool useBgColor) {
            if (_stateHelper.getLastState().fillPattern == SOLID_FILL || _stateHelper.getLastState().fillPattern == EMPTY_FILL){
                _propMan.setProp(state, PropertyName::TEXTURE_STATE, TextureState::NO_TEXTURE);
            } else {
                _propMan.setProp(state, PropertyName::FILL_PATTERN, _stateHelper.getLastState().fillPattern);
                _propMan.setProp(state, PropertyName::TEXTURE_STATE, TextureState::FILL_TEXTURE);
                if(_stateHelper.getLastState().fillPattern == USER_FILL){
                    _propMan.setPropP(
                            state,
                            PropertyName::USER_FILL_PATTERN,
                            _stateHelper.getLastState().userFillPattern
                    );
                }
                if(useBgColor){
                    _propMan.setProp(state, PropertyName::BG_COLOR, _stateHelper.getLastState().bgColor);
                }
            }
        }

        void BufferPreparer::useLineStyle(ItemState &state) {
            if(_stateHelper.getLastState().lineStyle == NULL_LINE){
                _propMan.setProp(state, PropertyName::TEXTURE_STATE, TextureState::NO_TEXTURE);
            }
        }

        void BufferPreparer::disableTexture(ItemState &state) {
            _propMan.setProp(state, PropertyName::TEXTURE_STATE, TextureState::NO_TEXTURE);
        }

        void BufferPreparer::disablePixelTexture(ItemState &state) {
            _propMan.setProp(state, PropertyName::PIXEL_TEXTURE_STATE, PixelTextureState::NO_TEXTURE);
        }

        void BufferPreparer::disableShader(ItemState &state) {
            _propMan.setProp(state, PropertyName::SHADER_TYPE, ShaderType::NO_SHADER);
        }

        ItemState BufferPreparer::createItemState(const QueueItem &item) {
            ItemState state = _propMan.getNullState();
            switch(item.type){
                case QueueItem::BAR:
                    disablePixelTexture(state);
                    useFillTexture(state, !_bufPrepParams->supportsTexturedBar());
                    if(_bufPrepParams->supportsTexturedBar() && _stateHelper.textureUsed()){
                        _propMan.setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_BAR_SHADER);
                    } else {
                        disableShader(state);
                    }
                    break;
                case QueueItem::SINGLE_PIXEL:
                    disablePixelTexture(state);
                    disableShader(state);
                    disableTexture(state);
                    break;
                case QueueItem::PIXEL_CONTAINER:
                    disableShader(state);
                    disableTexture(state);
                    _propMan.setPropP(state, PropertyName::PIXEL_CONTAINER, item.data.pixelContainer);
                    break;
                case QueueItem::FILLELLIPSE:
                    disablePixelTexture(state);
                    useFillTexture(state, true);
                    if(_stateHelper.getLastState().lineStyle == NULL_LINE){
                        if(_stateHelper.textureUsed()){
                            if(_bufPrepParams->supportsTexturedEllipse()){
                                _propMan.setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_ELLIPSE_SHADER);
                            }
                        } else {
                            if (_bufPrepParams->supportsEllipse()) {
                                _propMan.setProp(state, PropertyName::SHADER_TYPE, ShaderType::ELLIPSE_SHADER);
                            } else {
                                disableShader(state);
                            }
                        }
                    }
                    break;
                case QueueItem::CLEAR:
                    disablePixelTexture(state);
                    disableShader(state);
                    disableTexture(state);
                    break;
                default: break;
            }
            return state;
        }

        bool BufferPreparer::processStateDiff(const ItemState &statePrev, const ItemState &stateCur) {
            ItemState stateDiff = _propMan.itemStateDiff(statePrev, stateCur);
            bool isFirst = false;
            if(stateDiff[PropertyName::SHADER_TYPE].isSet){
                isFirst = true;
            }
            if(
                    stateDiff[PropertyName::TEXTURE_STATE].isSet &&
                    stateDiff[PropertyName::TEXTURE_STATE].val == TextureState::NO_TEXTURE
            ){
                _drawOps.push_back(DrawOpCreator::create<REMOVE_PATTERN_TEXTURE>());
                isFirst = true;
            }

            if(
                    stateDiff[PropertyName::PIXEL_TEXTURE_STATE].isSet &&
                    stateDiff[PropertyName::PIXEL_TEXTURE_STATE].val == PixelTextureState::NO_TEXTURE
            ){
                _drawOps.push_back(DrawOpCreator::create<REMOVE_PIXEL_TEXTURE>());
                isFirst = true;
            }

            if(stateDiff[PropertyName::USER_FILL_PATTERN].isSet){
                _drawOps.push_back(DrawOpCreator::create<SET_USER_FILL_PATTERN>(
                        static_cast<char*>(stateCur[PropertyName::USER_FILL_PATTERN].valP)
                ));
                isFirst = true;
            }
            if(_bufPrepParams->needRecreateTexture()){
                if (stateDiff[PropertyName::FILL_PATTERN].isSet || stateDiff[PropertyName::BG_COLOR].isSet){
                    _drawOps.push_back(
                            DrawOpCreator::create<SET_FILL_PATTERN_COLOR>(
                                    stateCur[PropertyName::FILL_PATTERN].val,
                                    stateCur[PropertyName::BG_COLOR].val
                            ));
                    isFirst = true;
                }
            } else {
                if (stateDiff[PropertyName::FILL_PATTERN].isSet) {
                    _drawOps.push_back(
                            DrawOpCreator::create<SET_FILL_PATTERN>(stateDiff[PropertyName::FILL_PATTERN].val));
                    isFirst = true;
                }
                if (stateDiff[PropertyName::BG_COLOR].isSet) {
                    _drawOps.push_back(DrawOpCreator::create<SET_TEX_BG_COLOR>(
                            stateDiff[PropertyName::BG_COLOR].val
                    ));
                    isFirst = true;
                }
            }
            if(stateDiff[PropertyName::PIXEL_CONTAINER].isSet){
                _drawOps.push_back(DrawOpCreator::create<SET_PIXEL_TEXTURE>(
                        static_cast<IPixelContainer*>(stateDiff[PropertyName::PIXEL_CONTAINER].valP)
                ));
                isFirst = true;
            }

            return isFirst;
        }

        BufferPreparer::TypeSize BufferPreparer::getTypeSize(const QueueItem &item) {
            TypeSize res;
            switch(item.type){
                case QueueItem::PIXEL_CONTAINER:
                    res.sizeMult = sizeof(TexturedVertex);
                    res.drawDataType = TEXTURED_VERTEX;
                    break;
                case QueueItem::BAR:
                    if (_stateHelper.textureUsed()) {
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
                case QueueItem::SINGLE_PIXEL: case QueueItem::CLEAR:
                    res.sizeMult = sizeof(ColorVertex);
                    res.drawDataType = COLOR_VERTEX;
                    break;
                case QueueItem::FILLELLIPSE:
                    if (_stateHelper.textureUsed()) {
                        if(_bufPrepParams->supportsTexturedEllipse()){
                            res.sizeMult = sizeof(TexturedColor2Vertex);
                            res.drawDataType = TEXTURED_ELLIPSE_VERTEX;
                        }
                    } else {
                        if (_bufPrepParams->supportsEllipse()) {
                            res.sizeMult = sizeof(ColorVertex);
                            res.drawDataType = ELLIPSE_VERTEX;
                        } else {
                            res.sizeMult = sizeof(ColorVertex);
                            res.drawDataType = COLOR_VERTEX;
                        }
                    }
                    break;
                default: break;
            }
            return res;
        }

        void BufferPreparer::genDegenerates(
                const QueueItem &item, void *&curVertMem
        ){
            if (!_isFirst) {
                if (_stateHelper.textureUsed()) {
                    switch(item.type) {
                        case QueueItem::BAR: {
                            if(_bufPrepParams->supportsTexturedBar()){
                                curVertMem = _primCreator.genFillCol2Degenerate(
                                        curVertMem, _prevCrds,
                                        _helper->toPixelsXY(item.data.bar.left, item.data.bar.top),
                                        genUCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight())
                                );
                            } else {
                                curVertMem = _primCreator.genFillDegenerate(
                                        curVertMem, _prevCrds,
                                        _helper->toPixelsXY(item.data.bar.left, item.data.bar.top)
                                );
                            }
                        }
                            break;
                        case QueueItem::FILLELLIPSE: {
                            if(_bufPrepParams->supportsTexturedEllipse()){
                                curVertMem = _primCreator.genTexEllipseDegenerate(
                                        curVertMem, _prevCrds,
                                        _helper->toPixelsXY(
                                                item.data.fillellipse.x - item.data.fillellipse.xradius,
                                                item.data.fillellipse.y - item.data.fillellipse.yradius
                                        ),
                                        genUCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight())
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
                                        curVertMem, _prevCrds,
                                        _helper->toPixelsXY(
                                                item.data.fillellipse.x - item.data.fillellipse.xradius,
                                                item.data.fillellipse.y - item.data.fillellipse.yradius
                                        ),
                                        genUCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight())
                                );
                            } else {
                                curVertMem = _primCreator.genDegenerate(
                                        curVertMem, _prevCrds,
                                        _helper->toPixelsXY(item.data.fillellipse.x,
                                                            item.data.fillellipse.y - item.data.fillellipse.yradius
                                        )
                                );
                            }
                        }
                            break;
                        case QueueItem::SINGLE_PIXEL:
                            curVertMem = _primCreator.genDegenerate(
                                    curVertMem, _prevCrds,
                                    genCoords(
                                    static_cast<int_fast32_t>(item.data.singlePixel.x),
                                    static_cast<int_fast32_t>(item.data.singlePixel.y)
                                    )
                            );
                            break;
                        case QueueItem::BAR:
                            curVertMem = _primCreator.genDegenerate(
                                    curVertMem, _prevCrds,
                                    _helper->toPixelsXY(item.data.bar.left, item.data.bar.top)
                            );
                            break;
                        case QueueItem::CLEAR:
                            curVertMem = _primCreator.genDegenerate(
                                    curVertMem, _prevCrds, genCoords(0, 0)
                            );
                            break;
                    }
                }
            }
        }

        void BufferPreparer::processDrawItem(
                const QueueItem &item, void *&curVertMem
        ) {
            genDegenerates(item, curVertMem);
            switch(item.type){
                case QueueItem::SINGLE_PIXEL:
                    curVertMem = _primCreator.genQuad(curVertMem,
                                                      genCoords(item.data.singlePixel.x, item.data.singlePixel.y),
                                                      genCoords(item.data.singlePixel.x + 1, item.data.singlePixel.y + 1),
                                                      item.data.singlePixel.color
                    );
                    _prevCrds = genCoords(item.data.singlePixel.x + 1, item.data.singlePixel.y + 1);
                    break;
                case QueueItem::CLEAR:
                    curVertMem = _primCreator.genQuad(curVertMem,
                                                      genCoords(0, 0),
                                                      genCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight()),
                                                      0xFFFFFF
                    );
                    _prevCrds = genCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight());
                    break;
                case QueueItem::FILLELLIPSE:
                    if (_stateHelper.textureUsed()) {
                        if (_bufPrepParams->supportsTexturedEllipse()) {
                            curVertMem = _primCreator.genTexEllipseQuad(curVertMem,
                                                                     _helper->toPixelsXY(item.data.fillellipse.x, item.data.fillellipse.y),
                                                                     _helper->toPixelsXYU(
                                                                                item.data.fillellipse.xradius,
                                                                                item.data.fillellipse.yradius
                                                                     ),
                                                                     _stateHelper.getLastState().fillColor,
                                                                     _stateHelper.getLastState().bgColor,
                                                                     genUCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight())
                            );
                            _prevCrds = _helper->toPixelsXY(
                                    item.data.fillellipse.x + item.data.fillellipse.xradius,
                                    item.data.fillellipse.y + item.data.fillellipse.yradius
                            );
                        }
                    } else {
                        if (_bufPrepParams->supportsEllipse()) {
                            curVertMem = _primCreator.genEllipseQuad(curVertMem,
                                                                     _helper->toPixelsXY(item.data.fillellipse.x, item.data.fillellipse.y),
                                                                     _helper->toPixelsXYU(
                                                                             item.data.fillellipse.xradius,
                                                                             item.data.fillellipse.yradius
                                                                     ),
                                                                     genUCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight()),
                                                                     _stateHelper.getFillColor()
                            );
                            _prevCrds = _helper->toPixelsXY(
                                    item.data.fillellipse.x + item.data.fillellipse.xradius,
                                    item.data.fillellipse.y + item.data.fillellipse.yradius
                            );
                        } else {
                            curVertMem = _primCreator.genEllipse(curVertMem,
                                                                 _helper->toPixelsXY(
                                                                         item.data.fillellipse.x,
                                                                         item.data.fillellipse.y
                                                                 ),
                                                                 _helper->toPixelsXYU(
                                                                         item.data.fillellipse.xradius,
                                                                         item.data.fillellipse.yradius
                                                                 ),
                                                                 _stateHelper.getFillColor()
                            );
                            _prevCrds = _helper->toPixelsXY(
                                    item.data.fillellipse.x, 
                                    item.data.fillellipse.y - item.data.fillellipse.yradius
                            );
                        }
                    }
                    break;
                case QueueItem::BAR:
                    if (_stateHelper.textureUsed()) {
                        if(_bufPrepParams->supportsTexturedBar()){
                            curVertMem = _primCreator.genFillCol2Quad(curVertMem,
                                                                  _helper->toPixelsXY(item.data.bar.left, item.data.bar.top),
                                                                  _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom),
                                                                  _stateHelper.getLastState().fillColor,
                                                                  _stateHelper.getLastState().bgColor,
                                                                  genUCoords(_bufPrepParams->getWidth(), _bufPrepParams->getHeight())

                            );
                        } else {
                            curVertMem = _primCreator.genFillQuad(curVertMem,
                                                                  _helper->toPixelsXY(item.data.bar.left, item.data.bar.top),
                                                                  _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom),
                                                                  _stateHelper.getLastState().fillColor
                            );
                        }
                    } else {
                        curVertMem = _primCreator.genQuad(curVertMem,
                                                          _helper->toPixelsXY(item.data.bar.left, item.data.bar.top),
                                                          _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom),
                                                          _stateHelper.getFillColor()

                        );
                    }
                    _prevCrds = _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom);
                    break;
                case QueueItem::PIXEL_CONTAINER: {
                    Rectangle coords = item.data.pixelContainer->getCoords();
                    curVertMem = _primCreator.genTexQuad(
                            curVertMem,
                            genCoords(coords.left, coords.top), genCoords(coords.right, coords.bottom),
                            genUCoords(_bufPrepParams->getPxTextureWidth(), _bufPrepParams->getPxTextureHeight())
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
            void *curVertMem = (static_cast<uint8_t*>(_vertMem) + _curUsedSize);
            if(_patterns.empty() && _stateHelper.getLastState().userFillPattern != NULL){
                _patterns.push_back(_stateHelper.getLastState().userFillPattern);
            }
            for (readIndex = offset; readIndex < size; readIndex++) {
                QueueItem &item = reader->getAt(readIndex);
                if (
                        item.type == QueueItem::SINGLE_PIXEL ||
                        item.type == QueueItem::BAR ||
                        item.type == QueueItem::CLEAR ||
                        item.type == QueueItem::PIXEL_CONTAINER ||
                        item.type == QueueItem::FILLELLIPSE
                ) {
                    ItemState lastState = createItemState(item);
                    _isFirst = processStateDiff(_stateHelper.getCurItemState(), lastState) || _isFirst;
                    _stateHelper.getCurItemState() = lastState;
                    TypeSize ts = getTypeSize(item);
                    uint_fast32_t curNumVertices;
                    if(item.type == QueueItem::FILLELLIPSE && !_bufPrepParams->supportsEllipse()){
                        curNumVertices = _primCreator.getNumEllipseVertices(
                                _helper->toPixelsXYU(
                                        item.data.fillellipse.xradius, 
                                        item.data.fillellipse.yradius
                                )
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
                    processDrawItem(item, curVertMem);
                } else {
                    switch (item.type){
                        case QueueItem::BGCOLOR:
                            _stateHelper.getLastState().bgColor = item.data.bgColor;
                            break;
                        case QueueItem::SETFILLSTYLE:
                            _stateHelper.getLastState().fillColor = item.data.setfillstyle.color;
                            _stateHelper.getLastState().fillPattern = item.data.setfillstyle.fillStyle;
                            break;
                        case QueueItem::SETFILLPATTERN:
                            _stateHelper.getLastState().fillPattern = USER_FILL;
                            _stateHelper.getLastState().userFillPattern = item.data.setfillpattern.fillPattern;
                            _stateHelper.getLastState().fillColor = item.data.setfillpattern.color;
                            _patterns.push_back(_stateHelper.getLastState().userFillPattern);
                            break;
                        case QueueItem::SETLINESTYLE:
                            _stateHelper.getLastState().lineStyle = item.data.setlinestyle.linestyle;
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
            _isFirst = true;
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
            delete [] _stateHelper.getLastState().userFillPattern;
        }

        bool BufferPreparer::isFull() {
            return !_canReadMore;
        }

        uint_fast32_t BufferPreparer::getLastOffset() {
            return _lastOffset;
        }

        void BufferPreparer::resetLastOffset() {
            _lastOffset = 0;
        }

        void BufferPreparer::endDraw() {
            _stateHelper.resetState();
        }
    }
}
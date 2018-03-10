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
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::START_TRANSPARENT_DATA>() {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::START_TRANSPARENT_DATA;
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
        BufferPreparer::DrawOp DrawOpCreator::create<BufferPreparer::SET_FILL_PATTERN_TWO_COLORS>(
                uint_fast32_t fillPattern, uint_fast32_t bgColor, uint_fast32_t fillColor
        ) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::SET_FILL_PATTERN_TWO_COLORS;
            op.data.fillPatternTwoColors.fillPattern = static_cast<uint8_t>(fillPattern);
            op.data.fillPatternTwoColors.bgColor = bgColor;
            op.data.fillPatternTwoColors.fillColor = fillColor;
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
                uint32_t size, uint32_t numItems, BufferPreparer::DrawDataType type
        ) {
            BufferPreparer::DrawOp op;
            op.type = BufferPreparer::ITEMS;
            op.data.items.size = size;
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
           _isFirst(true), _itemNum(_bufPrepParams->getMaxDepthValues() - 1),
           _curUsedSize(0), _lastOffset(0), _canReadMore(true)
        {
        }

        void BufferPreparer::useFillTexture(ItemState &state, bool useBgColor, bool useFillColorIfTransp) {
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
                    if(
                            useFillColorIfTransp &&
                            (
                                    color_has_alpha(_stateHelper.getLastState().fillColor) ||
                                    color_has_alpha(_stateHelper.getLastState().bgColor)
                            )
                    ){
                        _propMan.setProp(state, PropertyName::FILL_COLOR, _stateHelper.getLastState().fillColor);
                    }
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
                    useFillTexture(
                            state,
                            !_bufPrepParams->supportsTexturedBar(), _bufPrepParams->needRecreateTexture()
                    );
                    if(_bufPrepParams->supportsTexturedBar() && _stateHelper.fillTextureUsed(state)){
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
                    useFillTexture(
                            state,
                            !_bufPrepParams->supportsTexturedEllipse(),
                            _bufPrepParams->needRecreateTexture()
                    );
                    if(_stateHelper.getLastState().lineStyle == NULL_LINE){
                        if(_stateHelper.fillTextureUsed(state)){
                            if(_bufPrepParams->supportsTexturedEllipse()){
                                _propMan.setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_ELLIPSE_SHADER);
                            } else {
                                disableShader(state);
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

        bool BufferPreparer::isStateTransparent(const QueueItem &item, const ItemState & state) {
            if(item.type == QueueItem::PIXEL_CONTAINER){
                return true;
            }
            if (
                    (
                        item.type == QueueItem::BAR ||
                        item.type == QueueItem::FILLELLIPSE
                    ) && (
                        color_has_alpha(_stateHelper.getLastState().fillColor) ||
                        (_stateHelper.fillTextureUsed(state) && color_has_alpha(_stateHelper.getLastState().bgColor))
                    )
            ) {
                return true;
            }
            return false;
        }

        bool BufferPreparer::processStateDiff(
                const ItemState &statePrev, const ItemState &stateCur,
                DrawOpVector &drawOps, bool isTransp
        ) {
            ItemState stateDiff = _propMan.itemStateDiff(statePrev, stateCur);
            bool isFirst = false;
            if(stateDiff[PropertyName::SHADER_TYPE].isSet){
                isFirst = true;
            }
            if(
                    stateDiff[PropertyName::TEXTURE_STATE].isSet &&
                    stateDiff[PropertyName::TEXTURE_STATE].val == TextureState::NO_TEXTURE
            ){
                drawOps.push_back(DrawOpCreator::create<REMOVE_PATTERN_TEXTURE>());
                isFirst = true;
            }

            if(
                    stateDiff[PropertyName::PIXEL_TEXTURE_STATE].isSet &&
                    stateDiff[PropertyName::PIXEL_TEXTURE_STATE].val == PixelTextureState::NO_TEXTURE
            ){
                drawOps.push_back(DrawOpCreator::create<REMOVE_PIXEL_TEXTURE>());
                isFirst = true;
            }

            if(stateDiff[PropertyName::USER_FILL_PATTERN].isSet){
                drawOps.push_back(DrawOpCreator::create<SET_USER_FILL_PATTERN>(
                        static_cast<char*>(stateCur[PropertyName::USER_FILL_PATTERN].valP)
                ));
                isFirst = true;
            }
            if(_bufPrepParams->needRecreateTexture()){
                if (
                        stateDiff[PropertyName::FILL_PATTERN].isSet ||
                        stateDiff[PropertyName::BG_COLOR].isSet ||
                        (isTransp && stateDiff[PropertyName::FILL_COLOR].isSet)
                ){
                    if(isTransp){
                        drawOps.push_back(
                                DrawOpCreator::create<SET_FILL_PATTERN_TWO_COLORS>(
                                        stateCur[PropertyName::FILL_PATTERN].val,
                                        stateCur[PropertyName::BG_COLOR].val,
                                        stateCur[PropertyName::FILL_COLOR].val
                                ));
                    } else {
                        drawOps.push_back(
                                DrawOpCreator::create<SET_FILL_PATTERN_COLOR>(
                                        stateCur[PropertyName::FILL_PATTERN].val,
                                        stateCur[PropertyName::BG_COLOR].val
                                ));
                    }
                    isFirst = true;
                }
            } else {
                if (stateDiff[PropertyName::FILL_PATTERN].isSet) {
                    drawOps.push_back(
                            DrawOpCreator::create<SET_FILL_PATTERN>(stateDiff[PropertyName::FILL_PATTERN].val));
                    isFirst = true;
                }
                if (stateDiff[PropertyName::BG_COLOR].isSet) {
                    drawOps.push_back(DrawOpCreator::create<SET_TEX_BG_COLOR>(
                            stateDiff[PropertyName::BG_COLOR].val
                    ));
                    isFirst = true;
                }
            }
            if(stateDiff[PropertyName::PIXEL_CONTAINER].isSet){
                drawOps.push_back(DrawOpCreator::create<SET_PIXEL_TEXTURE>(
                        static_cast<IPixelContainer*>(stateDiff[PropertyName::PIXEL_CONTAINER].valP)
                ));
                isFirst = true;
            }

            return isFirst;
        }

        BufferPreparer::TypeSize BufferPreparer::getTypeSize(const QueueItem &item, const ItemState &state) {
            TypeSize res;
            switch(item.type){
                case QueueItem::PIXEL_CONTAINER:
                    res.sizeMult = sizeof(TexturedVertex);
                    res.drawDataType = TEXTURED_VERTEX;
                    break;
                case QueueItem::BAR:
                    if (_stateHelper.fillTextureUsed(state)) {
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
                    if (_stateHelper.fillTextureUsed(state)) {
                        if(_bufPrepParams->supportsTexturedEllipse()){
                            res.sizeMult = sizeof(TexturedColor2Vertex);
                            res.drawDataType = TEXTURED_ELLIPSE_VERTEX;
                        } else {
                            res.sizeMult = sizeof(TexturedColorVertex);
                            res.drawDataType = TEXTURED_COLOR_VERTEX;
                        }
                    } else {
                        if (_bufPrepParams->supportsEllipse()) {
                            res.sizeMult = sizeof(TexturedColorVertexNoRHW);
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
                const QueueItem &item, void *&curVertMem, const Coords &startCrds, const Coords &endCrds,
                const ItemState &state
        ){
            if (_stateHelper.fillTextureUsed(state)) {
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

        BufferPreparer::StartEndCoords BufferPreparer::getStartEndCoords(
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
                    crds.start = _helper->toPixelsXY(item.data.bar.left, item.data.bar.top);
                    crds.end = _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom);
                    break;
                case QueueItem::PIXEL_CONTAINER: {
                    Rectangle coords = item.data.pixelContainer->getCoords();
                    crds.start = genCoords(coords.left, coords.top);
                    crds.end = genCoords(coords.right, coords.bottom);
                }
                    break;
                case QueueItem::FILLELLIPSE:
                    if(
                            _stateHelper.fillTextureUsed(state) && _bufPrepParams->supportsTexturedEllipse() ||
                            !_stateHelper.fillTextureUsed(state) && _bufPrepParams->supportsEllipse()
                    ){
                        crds.start = _helper->toPixelsXY(
                                item.data.fillellipse.x - item.data.fillellipse.xradius,
                                item.data.fillellipse.y - item.data.fillellipse.yradius
                        );
                        crds.end = _helper->toPixelsXY(
                                item.data.fillellipse.x + item.data.fillellipse.xradius,
                                item.data.fillellipse.y + item.data.fillellipse.yradius
                        );
                    } else {
                        crds.start = crds.end = _helper->toPixelsXY(
                                item.data.fillellipse.x,
                                item.data.fillellipse.y - item.data.fillellipse.yradius
                        );
                    }

                    break;
            }
            return crds;
        }

        void BufferPreparer::processDrawItem(
                const QueueItem &item, void *&curVertMem,
                const ItemState &state
        ) {
            switch(item.type){
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
                    if (_stateHelper.fillTextureUsed(state)) {
                        if (_bufPrepParams->supportsTexturedEllipse()) {
                            curVertMem = _primCreator.genTexEllipseQuad(curVertMem,
                                                                     _helper->toPixelsXY(item.data.fillellipse.x, item.data.fillellipse.y),
                                                                     _helper->toPixelsXYU(
                                                                                item.data.fillellipse.xradius,
                                                                                item.data.fillellipse.yradius
                                                                     ),
                                                                     _curZ,
                                                                     _stateHelper.getLastState().fillColor,
                                                                     _stateHelper.getLastState().bgColor,
                                                                     _bufPrepParams->getMaxCoords()
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
                                                                 _curZ,
                                                                 _stateHelper.getLastState().fillColor,
                                                                 true
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
                                                                     _bufPrepParams->getMaxCoords(),
                                                                     _curZ,
                                                                     _stateHelper.getFillColor()
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
                                                                 _curZ,
                                                                 _stateHelper.getFillColor(),
                                                                 false
                            );
                        }
                    }
                    break;
                case QueueItem::BAR:
                    if (_stateHelper.fillTextureUsed(state)) {
                        if(_bufPrepParams->supportsTexturedBar()){
                            curVertMem = _primCreator.genFillCol2Quad(curVertMem,
                                                                  _helper->toPixelsXY(item.data.bar.left, item.data.bar.top),
                                                                  _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom),
                                                                  _curZ,
                                                                  _stateHelper.getLastState().fillColor,
                                                                  _stateHelper.getLastState().bgColor,
                                                                  _bufPrepParams->getMaxCoords()

                            );
                        } else {
                            curVertMem = _primCreator.genFillQuad(curVertMem,
                                                                  _helper->toPixelsXY(item.data.bar.left, item.data.bar.top),
                                                                  _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom),
                                                                  _curZ,
                                                                  _stateHelper.getLastState().fillColor
                            );
                        }
                    } else {
                        curVertMem = _primCreator.genQuad(curVertMem,
                                                          _helper->toPixelsXY(item.data.bar.left, item.data.bar.top),
                                                          _helper->toPixelsXY(item.data.bar.right, item.data.bar.bottom),
                                                          _curZ,
                                                          _stateHelper.getFillColor()

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

        uint_fast32_t BufferPreparer::getNumVertices(const QueueItem &item) {
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
            return curNumVertices;
        }

        void BufferPreparer::prepareBuffer(IQueueReader *reader, uint_fast32_t offset, uint_fast32_t maxSize) {
            uint_fast32_t size = reader->getSize();
            uint_fast32_t readIndex = 0;
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
                    bool isTransparent = isStateTransparent(item, lastState);
                    if(isTransparent){
                        _isFirst = processStateDiff(_stateHelper.getCurItemState(), lastState, _transpBuffer.drawOps, true)
                                   || _transpBuffer.vect.empty();
                        _stateHelper.getCurItemState() = lastState;
                    } else {
                        _isFirst = (_drawBuffers.find(lastState) == _drawBuffers.end());
                    }
                    TypeSize ts = getTypeSize(item, lastState);
                    uint_fast32_t curNumVertices = getNumVertices(item);
                    uint_fast32_t newUsedSize = _curUsedSize + curNumVertices * ts.sizeMult;
                    if (newUsedSize > maxSize) {
                        _canReadMore = false;
                        break;
                    }
                    _curUsedSize = newUsedSize;
                    _curZ = _itemNum * _bufPrepParams->getDepthIncrement();
                    if (isTransparent) {
                        void *curMem;
                        if(_transpBuffer.vect.empty()){
                            _transpBuffer.numItems = curNumVertices;
                            _transpBuffer.vect.resize(curNumVertices * ts.sizeMult);
                            curMem = reinterpret_cast<void *>(&_transpBuffer.vect[0]);
                        } else {
                            uint_fast32_t prevSize = _transpBuffer.vect.size();
                            _transpBuffer.numItems += curNumVertices;
                            _transpBuffer.vect.resize(_transpBuffer.vect.size() + curNumVertices * ts.sizeMult);
                            curMem = reinterpret_cast<void *>(&_transpBuffer.vect[0] + prevSize);
                        }
                        if(_isFirst){
                            _transpBuffer.drawOps.push_back(DrawOpCreator::create<ITEMS>(
                                    curNumVertices * ts.sizeMult,
                                    curNumVertices - VERTICES_TRIANGLES_DIFF,
                                    ts.drawDataType
                            ));
                        } else {
                            _transpBuffer.drawOps.back().data.items.numItems += curNumVertices;
                            _transpBuffer.drawOps.back().data.items.size += curNumVertices * ts.sizeMult;
                        }
                        StartEndCoords crds = getStartEndCoords(item, lastState);
                        if (!_isFirst) {
                            genDegenerates(item, curMem, _transpBuffer.prevCrds, crds.start, lastState);
                        }
                        processDrawItem(item, curMem, lastState);
                        _transpBuffer.prevCrds = crds.start;
                    } else {
                        ItemsBuffer &buffer = _drawBuffers[lastState];
                        void *curMem;
                        if (_isFirst) {
                            buffer.numItems = curNumVertices;
                            buffer.vect.resize(buffer.numItems * ts.sizeMult);
                            buffer.type = ts.drawDataType;
                            buffer.sizeMult = ts.sizeMult;
                            curMem = reinterpret_cast<void *>(&buffer.vect[0]);
                            buffer.offsets.push_back(0);
                        } else {
                            uint_fast32_t prevSize = buffer.vect.size();
                            buffer.numItems += curNumVertices;
                            buffer.vect.resize(buffer.vect.size() + curNumVertices * ts.sizeMult);
                            curMem = reinterpret_cast<void *>(&buffer.vect[0] + prevSize);
                            buffer.offsets.push_back(prevSize);
                        }
                        StartEndCoords crds = getStartEndCoords(item, lastState);
                        processDrawItem(item, curMem, lastState);
                        if (!_isFirst) {
                            genDegenerates(item, curMem, crds.end, buffer.prevCrds, lastState);
                        }
                        buffer.prevCrds = crds.start;
                    }
                    if(_itemNum == 0){
                        _canReadMore = false;
                        break;
                    }
                    _itemNum--;
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
            _lastOffset = readIndex;
        }

        void BufferPreparer::genOpsAndMemBlocks() {
            _drawOps.clear();
            _memBlocks.clear();
            _stateHelper.resetState();
            for(BufferMap::iterator it = _drawBuffers.begin(); it != _drawBuffers.end(); ++it){
                ItemState lastState = it->first;
                it->second.offsets.push_back(it->second.vect.size());
                for(uint_fast32_t i = it->second.offsets.size() - 2; ; i--){
                    MemBlock block = {
                            &it->second.vect[it->second.offsets[i]],
                            static_cast<uint_fast32_t>(it->second.offsets[i + 1] - it->second.offsets[i])
                    };
                    _memBlocks.push_back(block);
                    if(i == 0){
                        break;
                    }
                }
                processStateDiff(_stateHelper.getCurItemState(), lastState, _drawOps, false);
                _stateHelper.getCurItemState() = lastState;
                _drawOps.push_back(DrawOpCreator::create<ITEMS>(
                        it->second.vect.size(), it->second.numItems - VERTICES_TRIANGLES_DIFF, it->second.type
                ));
            }
            if(!_transpBuffer.vect.empty()) {
                MemBlock transpBlock = {&_transpBuffer.vect[0], _transpBuffer.vect.size()};
                _memBlocks.push_back(transpBlock);
                _drawOps.push_back(DrawOpCreator::create<START_TRANSPARENT_DATA>());
                std::copy(_transpBuffer.drawOps.begin(), _transpBuffer.drawOps.end(), std::back_inserter(_drawOps));
            }

        }

        void BufferPreparer::clear() {
            _stateHelper.resetState();
            _drawBuffers.clear();
            _transpBuffer.vect.clear();
            _transpBuffer.drawOps.clear();
            if(!_patterns.empty()){
                _patterns.pop_back();
                for(CharPVector::iterator it = _patterns.begin(); it != _patterns.end(); ++it){
                    delete [] *it;
                }
            }
            _patterns.clear();
            _curUsedSize = 0;
            _canReadMore = true;
            _isFirst = true;
            _itemNum = _bufPrepParams->getMaxDepthValues() - 1;
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
            return _curUsedSize == 0;
        }

        BufferPreparer::~BufferPreparer() {
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

        BufferPreparer::MemBlockVector::iterator BufferPreparer::memBlocksBegin() {
            return _memBlocks.begin();
        }

        BufferPreparer::MemBlockVector::iterator BufferPreparer::memBlocksEnd() {
            return _memBlocks.end();
        }

        uint_fast32_t BufferPreparer::getUsedSize() {
            return _curUsedSize;
        }
    }
}
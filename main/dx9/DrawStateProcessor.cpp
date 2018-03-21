#include "DrawStateProcessor.h"

namespace directgraph{
    namespace dx9{
        void DrawStateProcessor::useFillTexture(ItemState &state, bool useBgColor, bool useFillColorIfTransp) {
            if (_stateHelper->getLastState().fillPattern == SOLID_FILL || _stateHelper->getLastState().fillPattern == EMPTY_FILL){
                _propMan->setProp(state, PropertyName::TEXTURE_STATE, TextureState::NO_TEXTURE);
            } else {
                _propMan->setProp(state, PropertyName::FILL_PATTERN, _stateHelper->getLastState().fillPattern);
                _propMan->setProp(state, PropertyName::TEXTURE_STATE, TextureState::FILL_TEXTURE);
                if(_stateHelper->getLastState().fillPattern == USER_FILL){
                    _propMan->setPropP(
                            state,
                            PropertyName::USER_FILL_PATTERN,
                            _stateHelper->getLastState().userFillPattern
                    );
                }
                if(useBgColor){
                    _propMan->setProp(state, PropertyName::BG_COLOR, _stateHelper->getLastState().bgColor);
                    if(
                            useFillColorIfTransp &&
                            (
                                    color_has_alpha(_stateHelper->getLastState().fillColor) ||
                                    color_has_alpha(_stateHelper->getLastState().bgColor)
                            )
                    ){
                        _propMan->setProp(state, PropertyName::FILL_COLOR, _stateHelper->getLastState().fillColor);
                    }
                }
            }
        }

        void DrawStateProcessor::useLineStyle(ItemState &state, bool useDrawColorIfTransp) {
            if(_stateHelper->getLastState().lineStyle == NULL_LINE || _stateHelper->getLastState().lineStyle == SOLID_LINE){
                _propMan->setProp(state, PropertyName::TEXTURE_STATE, TextureState::NO_TEXTURE);
            } else {
                _propMan->setProp(state, PropertyName::TEXTURE_STATE, TextureState::LINE_TEXTURE);
                _propMan->setProp(state, PropertyName::LINE_PATTERN, _stateHelper->getLastState().lineStyle);
                if(useDrawColorIfTransp && color_has_alpha(_stateHelper->getLastState().drawColor)){
                    _propMan->setProp(state, PropertyName::DRAW_COLOR, _stateHelper->getLastState().drawColor);
                }
                if(_stateHelper->getLastState().lineStyle == USERBIT_LINE){
                    _propMan->setProp(
                            state,
                            PropertyName::USER_LINE_PATTERN,
                            _stateHelper->getLastState().userLinePattern
                    );
                }
            }
        }

        void DrawStateProcessor::disableTexture(ItemState &state) {
            _propMan->setProp(state, PropertyName::TEXTURE_STATE, TextureState::NO_TEXTURE);
        }

        void DrawStateProcessor::disablePixelTexture(ItemState &state) {
            _propMan->setProp(state, PropertyName::PIXEL_TEXTURE_STATE, PixelTextureState::NO_TEXTURE);
        }

        void DrawStateProcessor::disableShader(ItemState &state) {
            _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::NO_SHADER);
        }

        ItemState DrawStateProcessor::createItemState(const QueueItem &item) {
            ItemState state = _propMan->getNullState();
            switch(item.type){
                case QueueItem::BAR:
                    disablePixelTexture(state);
                    useFillTexture(
                            state,
                            !_bufPrepParams->supportsTexturedBar(), _bufPrepParams->needRecreateTexture()
                    );
                    if(_bufPrepParams->supportsTexturedBar() && _stateHelper->fillTextureUsed(state)){
                        _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_BAR_SHADER);
                    } else {
                        disableShader(state);
                    }
                    break;
                case QueueItem::RECTANGLE:
                    disablePixelTexture(state);
                    disableShader(state);
                    useLineStyle(state, _bufPrepParams->needRecreateTexture());
                    break;
                case QueueItem::SINGLE_PIXEL:
                    disablePixelTexture(state);
                    disableShader(state);
                    disableTexture(state);
                    break;
                case QueueItem::PIXEL_CONTAINER:
                    disableShader(state);
                    disableTexture(state);
                    _propMan->setProp(state, PropertyName::PIXEL_TEXTURE_STATE, PixelTextureState::PIXEL_TEXTURE);
                    _propMan->setPropP(state, PropertyName::PIXEL_CONTAINER, item.data.pixelContainer);
                    break;
                case QueueItem::FILLELLIPSE:
                    disablePixelTexture(state);
                    useFillTexture(
                            state,
                            !_bufPrepParams->supportsTexturedEllipse(),
                            _bufPrepParams->needRecreateTexture()
                    );
                    if(_stateHelper->getLastState().lineStyle == NULL_LINE){
                        if(_stateHelper->fillTextureUsed(state)){
                            if(_bufPrepParams->supportsTexturedEllipse()){
                                _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::TEXTURED_ELLIPSE_SHADER);
                            } else {
                                disableShader(state);
                            }
                        } else {
                            if (_bufPrepParams->supportsEllipse()) {
                                _propMan->setProp(state, PropertyName::SHADER_TYPE, ShaderType::ELLIPSE_SHADER);
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

        bool DrawStateProcessor::isStateTransparent(const QueueItem &item, const ItemState & state) {
            if(item.type == QueueItem::PIXEL_CONTAINER){
                return true;
            }
            if (
                    (
                            item.type == QueueItem::BAR ||
                            item.type == QueueItem::FILLELLIPSE
                    ) && (
                            color_has_alpha(_stateHelper->getLastState().fillColor) ||
                            (_stateHelper->fillTextureUsed(state) && color_has_alpha(_stateHelper->getLastState().bgColor))
                    )
                    ) {
                return true;
            }
            if(
                item.type == QueueItem::RECTANGLE &&
                        (
                                color_has_alpha(_stateHelper->getLastState().drawColor) ||
                                _stateHelper->lineTextureUsed(state)
                        )
            ){
                return true;
            }
            return false;
        }

        bool DrawStateProcessor::processStateDiff(
                const ItemState &statePrev, const ItemState &stateCur,
                DrawOpVector &drawOps, bool isTransp
        ) {
            ItemState stateDiff = _propMan->itemStateDiff(statePrev, stateCur);
            bool isFirst = false;
            if(stateDiff[PropertyName::SHADER_TYPE].isSet){
                isFirst = true;
            }
            if(
                    stateDiff[PropertyName::TEXTURE_STATE].isSet &&
                    stateDiff[PropertyName::TEXTURE_STATE].val == TextureState::NO_TEXTURE
                    ){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::REMOVE_PATTERN_TEXTURE>());
                isFirst = true;
            }

            if(
                    stateDiff[PropertyName::PIXEL_TEXTURE_STATE].isSet &&
                    stateDiff[PropertyName::PIXEL_TEXTURE_STATE].val == PixelTextureState::NO_TEXTURE
                    ){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::REMOVE_PIXEL_TEXTURE>());
                isFirst = true;
            }

            if(stateDiff[PropertyName::USER_FILL_PATTERN].isSet){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_USER_FILL_PATTERN>(
                        static_cast<char*>(stateCur[PropertyName::USER_FILL_PATTERN].valP)
                ));
                isFirst = true;
            }

            if(stateDiff[PropertyName::USER_LINE_PATTERN].isSet){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_USER_LINE_PATTERN>(
                        stateCur[PropertyName::USER_LINE_PATTERN].val
                ));
            }

            if(_bufPrepParams->needRecreateTexture()){
                if (
                        stateDiff[PropertyName::FILL_PATTERN].isSet ||
                        stateDiff[PropertyName::BG_COLOR].isSet ||
                        (isTransp && stateDiff[PropertyName::FILL_COLOR].isSet)
                        ){
                    if(isTransp){
                        drawOps.push_back(
                                DrawOpCreator::create<DrawOpType::SET_FILL_PATTERN_TWO_COLORS>(
                                        stateCur[PropertyName::FILL_PATTERN].val,
                                        stateCur[PropertyName::BG_COLOR].val,
                                        stateCur[PropertyName::FILL_COLOR].val
                                ));
                    } else {
                        drawOps.push_back(
                                DrawOpCreator::create<DrawOpType::SET_FILL_PATTERN_COLOR>(
                                        stateCur[PropertyName::FILL_PATTERN].val,
                                        stateCur[PropertyName::BG_COLOR].val
                                ));
                    }
                    isFirst = true;
                }
            } else {
                if(stateDiff[PropertyName::LINE_PATTERN].isSet || stateDiff[PropertyName::USER_LINE_PATTERN].isSet){
                    drawOps.push_back(
                            DrawOpCreator::create<DrawOpType::SET_LINE_PATTERN>(stateCur[PropertyName::LINE_PATTERN].val));
                    isFirst = true;
                } else {
                    if (stateDiff[PropertyName::FILL_PATTERN].isSet ||
                        stateDiff[PropertyName::USER_FILL_PATTERN].isSet) {
                        drawOps.push_back(
                                DrawOpCreator::create<DrawOpType::SET_FILL_PATTERN>(
                                        stateCur[PropertyName::FILL_PATTERN].val));
                        isFirst = true;
                    }
                    if (stateDiff[PropertyName::BG_COLOR].isSet) {
                        drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_TEX_BG_COLOR>(
                                stateDiff[PropertyName::BG_COLOR].val
                        ));
                        isFirst = true;
                    }
                }
            }
            if(stateDiff[PropertyName::PIXEL_CONTAINER].isSet){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_PIXEL_TEXTURE>(
                        static_cast<IPixelContainer*>(stateDiff[PropertyName::PIXEL_CONTAINER].valP)
                ));
                isFirst = true;
            }

            return isFirst;
        }

        DrawStateProcessor::DrawStateProcessor(StateHelper *stateHelper, BufferPreparerParams *bufPrepParams,
                                               PropertyManager *propMan)
            :_stateHelper(stateHelper), _bufPrepParams(bufPrepParams), _propMan(propMan)
        {

        }
    }
}

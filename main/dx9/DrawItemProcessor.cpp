#include <main/QueueItem.h>
#include "DrawItemProcessor.h"
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        bool DrawItemProcessor::processStateDiff(
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
                        stateDiff[PropertyName::USER_FILL_PATTERN].isSet ||
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

            if(
                    stateDiff[PropertyName::LINE_PATTERN].isSet ||
                    stateDiff[PropertyName::USER_LINE_PATTERN].isSet ||
                    (_bufPrepParams->needRecreateTexture() && stateDiff[PropertyName::DRAW_COLOR].isSet)
                    ){
                if(_bufPrepParams->needRecreateTexture() && stateDiff[PropertyName::DRAW_COLOR].isSet){
                    drawOps.push_back(
                            DrawOpCreator::create<DrawOpType::SET_LINE_PATTERN_COLOR>(
                                    stateCur[PropertyName::LINE_PATTERN].val,
                                    stateDiff[PropertyName::DRAW_COLOR].val
                            )
                    );
                } else {
                    drawOps.push_back(
                            DrawOpCreator::create<DrawOpType::SET_LINE_PATTERN>(
                                    stateCur[PropertyName::LINE_PATTERN].val
                            )
                    );
                }
                isFirst = true;
            }

            if(stateDiff[PropertyName::PIXEL_CONTAINER].isSet){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_PIXEL_TEXTURE>(
                        static_cast<IPixelContainer*>(stateDiff[PropertyName::PIXEL_CONTAINER].valP)
                ));
                isFirst = true;
            }

            return isFirst;
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
                StateHelper *stateHelper,
                BufferPreparerParams *bufPrepParams,
                DrawerManager *drawerManager,
                PropertyManager *propMan
        ): _stateHelper(stateHelper), _bufPrepParams(bufPrepParams),
           _drawerManager(drawerManager), _propMan(propMan)
        {
            resetItemCount();
        }

        float DrawItemProcessor::getCurZ() {
            return _curZ;
        }

        ItemState DrawItemProcessor::createItemState() {
            ItemState state = _propMan->getNullState();
            _drawerManager->getActiveDrawer()->getItemState(state);
            return state;
        }
    }
}

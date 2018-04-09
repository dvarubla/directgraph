#include <main/QueueItem.h>
#include "DrawItemProcessor.h"
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        bool DrawItemProcessor::processStateDiff(
                const ItemState &statePrev, const ItemState &stateCur,
                DrawOpVector &drawOps, bool isTransp
        ) {
            ItemStateDiff stateDiff = _propMan->getItemStateDiff(statePrev, stateCur);
            bool isFirst = false;
            if(addedOrChanged(stateDiff[PropertyName::SHADER_TYPE])){
                isFirst = true;
            }
            if(stateDiff[PropertyName::TEXTURE_STATE].removed){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::REMOVE_PATTERN_TEXTURE>());
                isFirst = true;
            }

            if(stateDiff[PropertyName::PIXEL_CONTAINER].removed){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::REMOVE_PIXEL_TEXTURE>());
                isFirst = true;
            }

            if(addedOrChanged(stateDiff[PropertyName::USER_FILL_PATTERN])){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_USER_FILL_PATTERN>(
                        static_cast<char*>(stateCur[PropertyName::USER_FILL_PATTERN].valP)
                ));
                isFirst = true;
            }

            if(addedOrChanged(stateDiff[PropertyName::USER_LINE_PATTERN])){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_USER_LINE_PATTERN>(
                        stateCur[PropertyName::USER_LINE_PATTERN].val
                ));
                isFirst = true;
            }

            if(_bufPrepParams->needRecreateTexture()){
                if (
                        addedOrChanged(stateDiff[PropertyName::FILL_PATTERN]) ||
                        addedOrChanged(stateDiff[PropertyName::USER_FILL_PATTERN]) ||
                        addedOrChanged(stateDiff[PropertyName::BG_COLOR]) ||
                        (isTransp && addedOrChanged(stateDiff[PropertyName::FILL_COLOR]))
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
                if (addedOrChanged(stateDiff[PropertyName::FILL_PATTERN]) ||
                    addedOrChanged(stateDiff[PropertyName::USER_FILL_PATTERN])) {
                    drawOps.push_back(
                            DrawOpCreator::create<DrawOpType::SET_FILL_PATTERN>(
                                    stateCur[PropertyName::FILL_PATTERN].val));
                    isFirst = true;
                }
                if (addedOrChanged(stateDiff[PropertyName::BG_COLOR])) {
                    drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_TEX_BG_COLOR>(
                            stateCur[PropertyName::BG_COLOR].val
                    ));
                    isFirst = true;
                }
            }

            if(
                    addedOrChanged(stateDiff[PropertyName::LINE_PATTERN]) ||
                    addedOrChanged(stateDiff[PropertyName::USER_LINE_PATTERN]) ||
                    (_bufPrepParams->needRecreateTexture() && addedOrChanged(stateDiff[PropertyName::DRAW_COLOR]))
                    ){
                if(_bufPrepParams->needRecreateTexture() && addedOrChanged(stateDiff[PropertyName::DRAW_COLOR])){
                    drawOps.push_back(
                            DrawOpCreator::create<DrawOpType::SET_LINE_PATTERN_COLOR>(
                                    stateCur[PropertyName::LINE_PATTERN].val,
                                    stateCur[PropertyName::DRAW_COLOR].val
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

            if(addedOrChanged(stateDiff[PropertyName::PIXEL_CONTAINER])){
                drawOps.push_back(DrawOpCreator::create<DrawOpType::SET_PIXEL_TEXTURE>(
                        static_cast<IPixelContainer*>(stateCur[PropertyName::PIXEL_CONTAINER].valP)
                ));
                isFirst = true;
            }

            return isFirst;
        }

        bool DrawItemProcessor::canCreateItems(uint_fast8_t itemsNum) {
            return _itemNum > itemsNum;
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

        bool DrawItemProcessor::addedOrChanged(const ItemStateDiffPart &diffPart) {
            return diffPart.added || diffPart.changed;
        }
    }
}

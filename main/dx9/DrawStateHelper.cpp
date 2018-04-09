#include <directgraph/directgraph_constants.h>
#include <main/util.h>
#include "DrawStateHelper.h"

namespace directgraph{
    namespace dx9{
        void DrawStateHelper::useFillTexture(ItemState &state, bool useBgColor, bool useFillColorIfTransp) {
            if (_stateHelper->getLastState().fillPattern != SOLID_FILL && _stateHelper->getLastState().fillPattern != EMPTY_FILL){
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

        void DrawStateHelper::useLineStyle(ItemState &state, bool useDrawColor) {
            if(_stateHelper->getLastState().lineStyle != NULL_LINE && _stateHelper->getLastState().lineStyle != SOLID_LINE){
                _propMan->setProp(state, PropertyName::TEXTURE_STATE, TextureState::LINE_TEXTURE);
                _propMan->setProp(state, PropertyName::LINE_PATTERN, _stateHelper->getLastState().lineStyle);
                if(useDrawColor){
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

        DrawStateHelper::DrawStateHelper(StateHelper *stateHelper, PropertyManager *propMan)
                : _stateHelper(stateHelper), _propMan(propMan) {

        }

        bool DrawStateHelper::isFillStateTransparent(const ItemState &state) {
            return
                    (!_stateHelper->fillTextureUsed(state) && color_has_alpha(_stateHelper->getFillColor())) ||
                    (_stateHelper->fillTextureUsed(state) && 
                            (color_has_alpha(_stateHelper->getLastState().bgColor) || 
                            color_has_alpha(_stateHelper->getLastState().fillColor))
                    )
            ;
        }
    }
}

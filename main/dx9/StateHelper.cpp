#include <directgraph/directgraph_constants.h>
#include "StateHelper.h"

namespace directgraph {
    namespace dx9 {

        StateHelper::DevDrawState &StateHelper::getLastState() {
            return _lastState;
        }

        StateHelper::StateHelper(PropertyManager *propMan, const CommonProps &props): _propMan(propMan) {
            _lastState.fillPattern = props.fillStyle;
            _lastState.bgColor = props.bgColor;
            _lastState.userFillPattern = props.userFillPattern;
            _lastState.lineStyle = props.lineStyle;
            _curItemState = _propMan->getInitialItemState();
        }

        uint_fast32_t StateHelper::getFillColor() {
            return (
                    (_lastState.fillPattern == SOLID_FILL) ?
                     _lastState.fillColor :
                     _lastState.bgColor
            );
        }

        bool StateHelper::fillTextureUsed(const ItemState &state) {
            return state[PropertyName::TEXTURE_STATE].val == TextureState::FILL_TEXTURE;
        }

        void StateHelper::resetState() {
            _curItemState = _propMan->getInitialItemState();
        }

        ItemState &StateHelper::getCurItemState() {
            return _curItemState;
        }
    }
}

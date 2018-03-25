#include <directgraph/directgraph_constants.h>
#include <main/util.h>
#include "StateHelper.h"

namespace directgraph {
    namespace dx9 {

        StateHelper::DevDrawState &StateHelper::getLastState() {
            return _lastState;
        }

        StateHelper::StateHelper(PropertyManager *propMan, const CommonProps &props): _propMan(propMan) {
            _lastState.fillPattern = props.fillStyle;
            _lastState.bgColor = props.bgColor;
            _lastState.drawColor = props.drawColor;
            _lastState.userFillPattern = props.userFillPattern;
            _lastState.lineStyle = props.lineStyle;
            _lastState.lineThickness = props.lineThickness;
            _lastState.userLinePattern = props.userLinePattern;
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
            return state[PropertyName::TEXTURE_STATE].isSet &&
                    state[PropertyName::TEXTURE_STATE].val == TextureState::FILL_TEXTURE;
        }

        bool StateHelper::lineTextureUsed(const ItemState &state) {
            return state[PropertyName::TEXTURE_STATE].isSet &&
                    state[PropertyName::TEXTURE_STATE].val == TextureState::LINE_TEXTURE;
        }

        void StateHelper::resetState() {
            _curItemState = _propMan->getInitialItemState();
        }

        ItemState &StateHelper::getCurItemState() {
            return _curItemState;
        }
    }
}

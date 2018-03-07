#include "PropertyManager.h"

namespace directgraph {
    namespace dx9 {

        PropertyManager::PropertyManager() {
        }

        ItemState PropertyManager::itemStateDiff(const ItemState &statePrev, const ItemState &stateCurr) {
            ItemState res;
            for(uint_fast8_t i = 0; i < PropertyName::TOTAL_PROP_COUNT; i++){
                if(stateCurr[i].isSet &&
                        (
                                !statePrev[i].isSet ||
                                (i == PropertyName::USER_FILL_PATTERN && stateCurr[i].valP != statePrev[i].valP) ||
                                (i != PropertyName::USER_FILL_PATTERN && stateCurr[i].val != statePrev[i].val)
                        )
                ){
                    res[i].isSet = true;
                    if(i == PropertyName::USER_FILL_PATTERN){
                        res[i].valP = stateCurr[i].valP;
                    } else {
                        res[i].val = stateCurr[i].val;
                    }
                } else {
                    res[i].isSet = false;
                }
            }
            return res;
        }

        ItemState PropertyManager::getInitialItemState() {
            ItemState res = getNullState();
            res[PropertyName::TEXTURE_STATE].val = TextureState::NO_TEXTURE;
            res[PropertyName::TEXTURE_STATE].isSet = true;
            res[PropertyName::PIXEL_TEXTURE_STATE].val = PixelTextureState::NO_TEXTURE;
            res[PropertyName::PIXEL_TEXTURE_STATE].isSet = true;
            res[PropertyName::SHADER_TYPE].val = ShaderType::NO_SHADER;
            res[PropertyName::SHADER_TYPE].isSet = true;
            return res;
        }

        ItemState PropertyManager::getNullState() {
            ItemState res;
            for(uint_fast8_t i = 0; i < PropertyName::TOTAL_PROP_COUNT; i++){
                res[i].isSet = false;
            }
            return res;
        }

        void PropertyManager::setProp(ItemState &state, PropertyName::Name name, uint_fast32_t val) {
            state[name].isSet = true;
            state[name].val = val;
        }

        void PropertyManager::setPropP(ItemState &state, PropertyName::Name name, void *valP) {
            state[name].isSet = true;
            state[name].valP = valP;
        }
    }
}

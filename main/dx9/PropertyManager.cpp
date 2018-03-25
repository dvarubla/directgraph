#include "PropertyManager.h"

namespace directgraph {
    namespace dx9 {

        PropertyManager::PropertyManager() {
        }

        ItemStateDiff PropertyManager::getItemStateDiff(const ItemState &statePrev, const ItemState &stateCurr) {
            ItemStateDiff res;
            for(uint_fast8_t i = 0; i < PropertyName::TOTAL_PROP_COUNT; i++){
                res[i].added = false;
                res[i].changed = false;
                res[i].removed = false;
                if(stateCurr[i].isSet){
                    if(statePrev[i].isSet){
                        if((i == PropertyName::USER_FILL_PATTERN && stateCurr[i].valP != statePrev[i].valP) ||
                           (i != PropertyName::USER_FILL_PATTERN && stateCurr[i].val != statePrev[i].val)){
                            res[i].changed = true;
                        }
                    } else {
                        res[i].added = true;
                    }
                } else if(statePrev[i].isSet){
                    res[i].removed = true;
                }
            }
            return res;
        }

        ItemState PropertyManager::getInitialItemState() {
            ItemState res = getNullState();
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

        std::size_t ItemStateHash::operator()(const ItemState &state) const {
            std::size_t res = 0;
            for(uint_fast8_t i = 0; i < PropertyName::TOTAL_PROP_COUNT; i++){
                res <<= 1;
                if(state[i].isSet) {
                    if (i == PropertyName::USER_FILL_PATTERN) {
                        res ^= reinterpret_cast<std::size_t>(state[i].valP);
                    } else {
                        res ^= state[i].val;
                    }
                }
            }
            return res;
        }

        bool operator==(const ItemState &l, const ItemState &r){
            for(uint_fast8_t i = 0; i < PropertyName::TOTAL_PROP_COUNT; i++){
                if( (l[i].isSet && !r[i].isSet) || (!l[i].isSet && r[i].isSet)){
                    return false;
                }
                if(l[i].isSet) {
                    if (i == PropertyName::USER_FILL_PATTERN) {
                        if(l[i].valP != r[i].valP){
                            return false;
                        }
                    } else {
                        if(l[i].val != r[i].val){
                            return false;
                        }
                    }
                }
            }
            return true;
        }
    }
}

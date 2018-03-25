#pragma once

#include "PropertyManager.h"
#include "StateHelper.h"

namespace directgraph {
    namespace dx9 {
        class DrawStateHelper {
        private:
            StateHelper *_stateHelper;
            PropertyManager *_propMan;
        public:
            DrawStateHelper(StateHelper *stateHelper, PropertyManager *propMan);
            void useFillTexture(ItemState &state, bool useBgColor, bool useFillColorIfTransp);
            void disablePixelTexture(ItemState &state);
            void disableTexture(ItemState &state);
            void useLineStyle(ItemState &state, bool useDrawColor);
            void disableShader(ItemState &state);

            bool isFillStateTransparent(const ItemState &state);
        };
    }
}
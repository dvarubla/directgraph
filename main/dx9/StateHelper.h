#pragma once

#include <stdint.h>
#include <main/CommonProps.h>
#include <PropertyManager.h>

namespace directgraph {
    namespace dx9 {
        class StateHelper {
        public:
            struct DevDrawState{
                uint_fast8_t fillPattern;
                char *userFillPattern;
                uint_fast8_t lineStyle;
                uint_fast32_t bgColor;
                uint_fast32_t drawColor;
                uint_fast32_t fillColor;
                uint_fast32_t lineThickness;
                uint_fast16_t userLinePattern;
            };
        private:
            PropertyManager *_propMan;
            DevDrawState _lastState;
            ItemState _curItemState;
        public:
            DevDrawState & getLastState();

            ItemState & getCurItemState();

            StateHelper(PropertyManager *propMan, const CommonProps &props);

            uint_fast32_t getFillColor();

            void resetState();

            bool fillTextureUsed(const ItemState &state);

            bool lineTextureUsed(const ItemState &state);
        };
    }
}


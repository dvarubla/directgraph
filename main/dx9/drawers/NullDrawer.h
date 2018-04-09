#pragma once

#include "IDrawer.h"

namespace directgraph {
    namespace dx9 {
        class NullDrawer: public IDrawer {
        public:
            bool isSemiTransparent();

            void setItemState(const ItemState &state);

            void setItem(const QueueItem &item);

            uint_fast8_t getNumStages();

            void setStage(uint_fast8_t stage);

            uint_fast32_t getTotalSize();

            ~NullDrawer();
        };
    }
}



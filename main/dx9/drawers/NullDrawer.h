#pragma once

#include "IDrawer.h"

namespace directgraph {
    namespace dx9 {
        class NullDrawer: public IDrawer {
        public:
            bool isSemiTransparent();

            void setItemState(const ItemState &state);

            void setItem(const QueueItem &item);

            ~NullDrawer();
        };
    }
}



#include "NullDrawer.h"

namespace directgraph{
    namespace dx9{
        bool NullDrawer::isSemiTransparent() {
            return false;
        }

        NullDrawer::~NullDrawer() {
        }

        void NullDrawer::setItemState(const ItemState &) {
        }

        void NullDrawer::setItem(const QueueItem &) {
        }
    }
}
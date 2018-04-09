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

        uint_fast8_t NullDrawer::getNumStages() {
            return 1;
        }

        void NullDrawer::setStage(uint_fast8_t) {
        }

        uint_fast32_t NullDrawer::getTotalSize() {
            NumVertices nv = getNumVertices(false);
            return (nv.degenerate + nv.primitive) * getTypeSize().sizeMult;
        }
    }
}
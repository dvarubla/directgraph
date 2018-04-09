#pragma once

#include "StateHelper.h"
#include "DrawOpCreator.h"
#include "BufferPreparerParams.h"
#include "misc.h"
#include "DrawerManager.h"
#include "PropertyManager.h"
#include <main/QueueItem.h>
#include <main/DPIHelper.h>

namespace directgraph {
    namespace dx9 {
        class DrawItemProcessor {
        private:
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            DrawerManager *_drawerManager;
            PropertyManager *_propMan;
            float _curZ;
            uint_fast32_t _itemNum;

            void setCurZ();
            bool addedOrChanged(const ItemStateDiffPart &diffPart);
        public:
            DrawItemProcessor(
                    StateHelper *stateHelper,
                    BufferPreparerParams *bufPrepParams,
                    DrawerManager *drawerManager,
                    PropertyManager *propMan
            );
            bool canCreateItems(uint_fast8_t itemsNum);
            void nextItem();
            void resetItemCount();
            float getCurZ();
            bool processStateDiff(
                    const ItemState &statePrev, const ItemState &stateCur,
                    DrawOpVector &drawOps, bool isTransp
            );
            ItemState createItemState();
        };
    }
}


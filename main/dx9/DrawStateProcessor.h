#pragma once

#include <main/QueueItem.h>
#include <vector>
#include "PropertyManager.h"
#include "DrawOpCreator.h"
#include "BufferPreparerParams.h"
#include "StateHelper.h"

namespace directgraph {
    namespace dx9 {
        class DrawStateProcessor {
        private:
            StateHelper *_stateHelper;
            BufferPreparerParams *_bufPrepParams;
            PropertyManager *_propMan;
        public:
            DrawStateProcessor(StateHelper *stateHelper, BufferPreparerParams *bufPrepParams, PropertyManager *propMan);
            ItemState createItemState(const QueueItem &item);
            bool isStateTransparent(const QueueItem &item, const ItemState &state);
            bool processStateDiff(
                    const ItemState &statePrev, const ItemState &stateCur,
                    DrawOpVector &drawOps, bool isTransp
            );

            void useFillTexture(ItemState &state, bool useBgColor, bool useFillColorIfTransp);
            void disablePixelTexture(ItemState &state);
            void disableTexture(ItemState &state);
            void useLineStyle(ItemState &state, bool useDrawColor);
            void disableShader(ItemState &state);
        };
    }
}



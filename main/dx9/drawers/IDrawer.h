#pragma once

#include <PropertyManager.h>
#include <misc.h>
#include <main/QueueItem.h>

namespace directgraph {
    namespace dx9 {
        class IDrawer {
        public:
            virtual void getItemState(ItemState &state) = 0;
            virtual NumVertices getNumVertices(bool isFirst) = 0;
            virtual TypeSize getTypeSize() = 0;
            virtual void processDrawItem(
                    void *&curVertMem,
                    uint_fast32_t &numVertices,
                    float curZ
            ) = 0;
            virtual StartEndCoords getStartEndCoords(
            ) = 0;
            virtual void genDegenerates(
                    void *&curVertMem,
                    const FCoords &startCrds, const FCoords &endCrds,
                    float curZ
            ) = 0;
            virtual bool isSemiTransparent() = 0;
            virtual void setItem(const QueueItem &item) = 0;
            virtual void setItemState(const ItemState &state) = 0;
            virtual ~IDrawer() {}
        };
    }
}



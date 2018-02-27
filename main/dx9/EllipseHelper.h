#pragma once

#include <stdint.h>
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        class EllipseHelper {
        private:
            const static float EXTRA_OFFSET;
            uint_fast32_t convertIndex(uint_fast32_t index);
        public:
            EllipseHelper();
            uint_fast32_t getNumEllipseVertices(
                    uint_fast32_t rx,
                    uint_fast32_t ry
            );
            ColorVertex* genEllipse(
                    void *memoryVoid,
                    int_fast32_t centerX,
                    int_fast32_t centerY,
                    uint_fast32_t rx,
                    uint_fast32_t ry,
                    uint_fast32_t color,
                    int_fast32_t &prevX,
                    int_fast32_t &prevY
            );
        };
    }
}



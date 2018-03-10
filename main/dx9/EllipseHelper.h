#pragma once

#include <stdint.h>
#include <main/util.h>
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        class EllipseHelper {
        private:
            const static float EXTRA_OFFSET;
            uint_fast32_t convertIndex(uint_fast32_t index);
            uint_fast32_t _color;
            float _z;
            double _minX;
            double _minY;
            ColorVertex createVertex(double x, double y);
            TexturedColorVertex createTexturedVertex(double x, double y);
        public:
            EllipseHelper();
            uint_fast32_t getNumEllipseVertices(
                    uint_fast32_t rx,
                    uint_fast32_t ry
            );
            void* genEllipse(
                    void *memoryVoid,
                    Coords centerCrds,
                    UCoords radiusCrds,
                    float z,
                    uint_fast32_t color,
                    bool textured
            );
        };
    }
}



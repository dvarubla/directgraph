#pragma once

#include <stdint.h>
#include <main/util.h>
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        class EllipseHelper {
        private:
            const static float EXTRA_OFFSET;
            uint_fast32_t _color;
            float _z;
            double _minX;
            double _minY;
            ColorVertex createVertex(double x, double y);
            TexturedColorVertex createTexturedVertex(double x, double y);
            void createVertices(void *memory, double x, double y, uint_fast32_t index, bool textured);
        public:
            EllipseHelper();
            uint_fast32_t getNumEllipseVertices(
                    const UCoords & radiusCrds
            );
            void* genEllipse(
                    void *memoryVoid,
                    const Coords & centerCrds,
                    const UCoords & radiusCrds,
                    float z,
                    uint_fast32_t color,
                    bool textured
            );
        };
    }
}


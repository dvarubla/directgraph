#pragma once

#include <stdint.h>
#include "EllipseHelper.h"
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        class PrimitiveCreator {
        private:
            EllipseHelper _ellipseHelper;
        public:
            PrimitiveCreator();

            RectVertex *genDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY
            );

            TexturedVertex *genTexDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY
            );

            RectVertex *genQuad(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t color
            );

            TexturedRectVertex *genFillDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY
            );

            TexturedRectVertex *genFillQuad(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t color
            );

            TexturedVertex *genTexQuad(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t maxX, uint_fast32_t maxY
            );

            EllipseVertex *genEllipseDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t width, uint_fast32_t height
            );

            EllipseVertex *genEllipseQuad(
                    void *verticesVoid,
                    int_fast32_t centerX, int_fast32_t centerY,
                    int_fast32_t radiusX, int_fast32_t radiusY,
                    uint_fast32_t width, uint_fast32_t height,
                    uint_fast32_t color
            );

            RectVertex* genEllipse(
                    void *verticesVoid,
                    int_fast32_t centerX,
                    int_fast32_t centerY,
                    uint_fast32_t rx,
                    uint_fast32_t ry,
                    uint_fast32_t color,
                    int_fast32_t &prevX,
                    int_fast32_t &prevY
            );

            uint_fast32_t getNumEllipseVertices(
                    uint_fast32_t rx,
                    uint_fast32_t ry
            );
        };
    }
}



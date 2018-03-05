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

            ColorVertex *genDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY
            );

            TexturedVertex *genTexDegenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY
            );

            ColorVertex *genQuad(
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

            Color2Vertex *genFillCol2Degenerate(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t width, uint_fast32_t height
            );

            Color2Vertex *genFillCol2Quad(
                    void *verticesVoid,
                    int_fast32_t startX, int_fast32_t startY,
                    int_fast32_t endX, int_fast32_t endY,
                    uint_fast32_t color1, uint_fast32_t color2,
                    uint_fast32_t width, uint_fast32_t height
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

            ColorVertex* genEllipse(
                    void *verticesVoid,
                    int_fast32_t centerX,
                    int_fast32_t centerY,
                    uint_fast32_t rx,
                    uint_fast32_t ry,
                    uint_fast32_t color
            );

            uint_fast32_t getNumEllipseVertices(
                    uint_fast32_t rx,
                    uint_fast32_t ry
            );
        };
    }
}



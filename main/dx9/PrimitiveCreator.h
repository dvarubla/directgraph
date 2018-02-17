#pragma once

#include <windows.h>
#include <stdint.h>

namespace directgraph {
    namespace dx9 {
        class PrimitiveCreator {
        private:
        public:
            PrimitiveCreator();

            struct EllipseVertex {
                float x, y, z, rhw;
                DWORD color;
            };

            struct RectVertex {
                float x, y, z, rhw;
                DWORD color;
            };
            struct TexturedVertex {
                float x, y, z, rhw;
                float tu, tv;
            };
            struct TexturedRectVertex {
                float x, y, z, rhw;
                DWORD color;
                float tu, tv;
            };

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
        };

        class VertexCreator {
        public:
            template<typename T>
            static T create(
                    float x, float y, float z, float rhw, DWORD color
            );

            template<typename T>
            static T create(float x, float y, float z, float rhw, float tu, float tv);

            template<typename T>
            static T create(float x, float y, float z, float rhw, DWORD color, float tu, float tv);
        };
    }
}



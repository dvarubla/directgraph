#pragma once
#include <windows.h>

namespace directgraph {
    namespace dx9 {
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


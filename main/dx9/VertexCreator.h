#pragma once
#include <windows.h>

namespace directgraph {
    namespace dx9 {
        struct ColorVertex {
            float x, y, z, rhw;
            DWORD color;
        };

        struct Color2Vertex {
            float x, y, z;
            DWORD color1;
            DWORD color2;
            float tu, tv;
        };

        struct TexturedVertex {
            float x, y, z, rhw;
            float tu, tv;
        };
        
        struct TexturedColorVertex {
            float x, y, z, rhw;
            DWORD color;
            float tu, tv;
        };

        struct TexturedColorVertexNoRHW {
            float x, y, z;
            DWORD color;
            float tu, tv;
        };
        
        struct TexturedColor2Vertex {
            float x, y, z;
            DWORD color1;
            DWORD color2;
            float tu, tv, t3, t4;
        };

        class VertexCreator {
        public:
            template<typename T>
            static T create(
                    float x, float y, float z, float rhw, DWORD color
            );

            template<typename T>
            static T create(
                    float x, float y, float z, DWORD color1, DWORD color2, float tu, float tv
            );

            template<typename T>
            static T create(
                    float x, float y, float z, DWORD color, float tu, float tv
            );
            
            template<typename T>
            static T create(float x, float y, float z, float rhw, float tu, float tv);

            template<typename T>
            static T create(float x, float y, float z, float rhw, DWORD color, float tu, float tv);

            template<typename T>
            static T create(float x, float y, float z, DWORD color1, DWORD color2, float tu, float tv, float t3, float t4);
        };
    }
}


#pragma once

#include "DrawOpCreator.h"

namespace directgraph {
    namespace dx9 {
        const int TRIANGLES_IN_QUAD = 2;
        const int VERTICES_IN_QUAD = 4;
        struct StartEndCoords {
            Coords start;
            Coords end;
        };
        struct TextureCoords{
            FCoords start;
            FCoords end;
        };
        struct TypeSize{
            uint_fast32_t sizeMult;
            DrawDataType::Type drawDataType;
        };
        struct NumVertices{
            uint_fast32_t degenerate;
            uint_fast32_t primitive;
        };
    }
}


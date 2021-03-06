#pragma once

#include "DrawOpCreator.h"
#include <array>
#include <main/Vect.h>

namespace directgraph {
    namespace dx9 {
        const int TRIANGLES_IN_QUAD = 2;
        const int VERTICES_IN_QUAD = 4;
        struct StartEndCoords {
            FCoords start;
            FCoords end;
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

        typedef std::array<FCoords, VERTICES_IN_QUAD> QuadPointsArr;

        typedef std::vector<FCoords> CoordVect;
        typedef std::vector<Coords> ICoordVect;

        typedef struct{
            CoordVect coords;
            CoordVect texCoords;
        } Polyline, Polygon, Ellipse;

        struct EllipseOutline{
            CoordVect coords;
        };

        struct FullEllipse{
            Ellipse ellipse;
            EllipseOutline outline;
        };

        struct PolylinePolygon{
            Polyline polyline;
            Polygon polygon;
        };

        extern const float CORR_OFFSET;
    }
}


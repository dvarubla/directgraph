#include <VertexCreator.h>
#include "DegenerateHelper.h"

namespace directgraph {
    namespace dx9 {
        void * DegenerateHelper::genDegenerate(
                void *verticesVoid,
                const Coords &startCrds,
                const Coords &endCrds,
                float z
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f, 0
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f, 0
            );
            vertices++;
            return vertices;
        }

        void * DegenerateHelper::genTexDegenerate(
                void *verticesVoid,
                const Coords &startCrds,
                const Coords &endCrds,
                float z
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f, 0,
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f, 0,
                    0.0f,
                    0.0f
            );
            vertices++;
            return vertices;
        }

        void *DegenerateHelper::genTexCol2Degenerate(
                void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                float z
        ) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
                    z, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void *
        DegenerateHelper::genTexRectangleDegenerate(
                void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                float z
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x), static_cast<float>(startCrds.y),
                    z, 0.f, 0, 0, 0
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x), static_cast<float>(endCrds.y),
                    z, 0.f, 0, 0, 0
            );
            vertices++;
            return vertices;
        }

        void *DegenerateHelper::genEllipseDegenerate(void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                                                     float z) {
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z, 0ul, 0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
                    z, 0ul, 0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void *
        DegenerateHelper::genTexEllipseDegenerate(void *verticesVoid, const Coords &startCrds, const Coords &endCrds,
                                                  float z) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(startCrds.x),
                    static_cast<float>(startCrds.y),
                    z, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    static_cast<float>(endCrds.x),
                    static_cast<float>(endCrds.y),
                    z, 0ul, 0ul, 0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }
    }
}

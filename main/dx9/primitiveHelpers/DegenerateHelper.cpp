#include <VertexCreator.h>
#include "DegenerateHelper.h"

namespace directgraph {
    namespace dx9 {
        void * DegenerateHelper::genDegenerate(
                void *verticesVoid,
                const FCoords &startCrds,
                const FCoords &endCrds,
                float z
        ) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<ColorVertex>(
                    startCrds.x,
                    startCrds.y,
                    z, 1.0f, 0
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    endCrds.x,
                    endCrds.y,
                    z, 1.0f, 0
            );
            vertices++;
            return vertices;
        }

        void * DegenerateHelper::genTexDegenerate(
                void *verticesVoid,
                const FCoords &startCrds,
                const FCoords &endCrds,
                float z
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertex>(
                    startCrds.x,
                    startCrds.y,
                    z, 1.0f, 0,
                    0.0f,
                    0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    endCrds.x,
                    endCrds.y,
                    z, 1.0f, 0,
                    0.0f,
                    0.0f
            );
            vertices++;
            return vertices;
        }

        void *DegenerateHelper::genTexCol2Degenerate(
                void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds,
                float z
        ) {
            Color2VertexNoRHW *vertices = static_cast<Color2VertexNoRHW*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2VertexNoRHW>(
                    startCrds.x,
                    startCrds.y,
                    z, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    endCrds.x,
                    endCrds.y,
                    z, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void *
        DegenerateHelper::genShaderRectangleDegenerate(
                void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds,
                float z
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    startCrds.x, startCrds.y,
                    z, 0.f, 0, 0, 0
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    endCrds.x, endCrds.y,
                    z, 0.f, 0, 0, 0
            );
            vertices++;
            return vertices;
        }

        void *DegenerateHelper::genTexColorNoRHWDegenerate(void *verticesVoid, const FCoords &startCrds,
                                                           const FCoords &endCrds,
                                                           float z) {
            TexturedColorVertexNoRHW *vertices = static_cast<TexturedColorVertexNoRHW*>(verticesVoid);
            (*vertices) = VertexCreator::create<TexturedColorVertexNoRHW>(
                    startCrds.x,
                    startCrds.y,
                    z, 0ul, 0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertexNoRHW>(
                    endCrds.x,
                    endCrds.y,
                    z, 0ul, 0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void *
        DegenerateHelper::genTexEllipseDegenerate(void *verticesVoid, const FCoords &startCrds, const FCoords &endCrds,
                                                  float z) {
            Color2VertexNoRHW *vertices = static_cast<Color2VertexNoRHW*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2VertexNoRHW>(
                    startCrds.x,
                    startCrds.y,
                    z, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2VertexNoRHW>(
                    endCrds.x,
                    endCrds.y,
                    z, 0ul, 0ul, 0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void *DegenerateHelper::genEllipseWithOutlineDegenerate(void *verticesVoid, const FCoords &startCrds,
                                                                const FCoords &endCrds, float z) {
            Color2Vertex *vertices = static_cast<Color2Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color2Vertex>(
                    startCrds.x,
                    startCrds.y,
                    z, 0.0, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color2Vertex>(
                    endCrds.x,
                    endCrds.y,
                    z, 0.0, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }

        void *DegenerateHelper::genTexEllipseWithOutlineDegenerate(void *verticesVoid, const FCoords &startCrds,
                                                                const FCoords &endCrds, float z) {
            Color3Vertex *vertices = static_cast<Color3Vertex*>(verticesVoid);
            (*vertices) = VertexCreator::create<Color3Vertex>(
                    startCrds.x,
                    startCrds.y,
                    z, 0.0, 0ul, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            *vertices = VertexCreator::create<Color3Vertex>(
                    endCrds.x,
                    endCrds.y,
                    z, 0.0, 0ul, 0ul, 0ul,
                    0.0f, 0.0f
            );
            vertices++;
            return vertices;
        }
    }
}

#include "SimplePrimHelper.h"
#include "VertexCreator.h"

namespace directgraph {
    namespace dx9 {
        void *SimplePrimHelper::genQuad(void *verticesVoid, const Coords &startCrds, const Coords &endCrds, float z,
                                        uint_fast32_t color) {
            ColorVertex *vertices = static_cast<ColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            *vertices = VertexCreator::create<ColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color)
            );
            vertices++;
            return vertices;
        }

        void *
        SimplePrimHelper::genTexColorQuad(
                void *verticesVoid, const Coords &startCrds, const Coords &endCrds, float z,
                uint_fast32_t color, const TextureCoords &textureCoords, bool rotate
        ) {
            TexturedColorVertex *vertices = static_cast<TexturedColorVertex*>(verticesVoid);
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureCoords.start.x,
                    textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(startCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.start.x : textureCoords.end.x,
                    (rotate) ? textureCoords.end.y : textureCoords.start.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(startCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    (rotate) ? textureCoords.end.x : textureCoords.start.x,
                    (rotate) ? textureCoords.start.y : textureCoords.end.y
            );
            vertices++;
            *vertices = VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(endCrds.x) - 0.5f,
                    static_cast<float>(endCrds.y) - 0.5f,
                    z, 1.0f,
                    swap_color_transp(color),
                    textureCoords.end.x,
                    textureCoords.end.y
            );
            vertices++;
            return vertices;
        }
    }
}

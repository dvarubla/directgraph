#include <algorithm>
#include <main/patterns.h>
#include "EllipseHelper.h"
#include "util.h"

#undef min

namespace directgraph{
    namespace dx9{
        const float EllipseHelper::EXTRA_OFFSET = 0.005f;

        EllipseHelper::EllipseHelper() {

        }

        void EllipseHelper::createVertices(void *memory, double x, double y, uint_fast32_t index, bool textured) {
            ColorVertex *memoryCol;
            TexturedColorVertex *memoryTextured;
            if(textured){
                memoryTextured = static_cast<TexturedColorVertex *>(memory);
            } else {
                memoryCol = static_cast<ColorVertex *>(memory);
            }
            if(textured){
                memoryTextured[index * 2 - 1] = createTexturedVertex(x, y);
            } else {
                memoryCol[index * 2 - 1] = createVertex(x, y);
            }
            if(index % 2 == 0){
                if(textured){
                    memoryTextured[index * 2 - 2] = createTexturedVertex(x, y);
                } else {
                    memoryCol[index * 2 - 2] = createVertex(x, y);
                }
            }
        }

        void *EllipseHelper::genEllipse(
                void *memoryVoid,
                const Coords & centerCrds,
                const UCoords & radiusCrds,
                float z,
                uint_fast32_t color,
                bool textured
        ) {
            _z = z;
            _color = swap_color_transp(color);
            if(textured) {
                _minX = static_cast<double>(centerCrds.x - static_cast<int_fast32_t>(radiusCrds.x));
                _minY = static_cast<double>(centerCrds.y - static_cast<int_fast32_t>(radiusCrds.y));
            }
            
            bool swap;
            uint_fast32_t minR, maxR;
            if(radiusCrds.y > radiusCrds.x){
                maxR = radiusCrds.y;
                minR = radiusCrds.x;
                swap = false;
            } else {
                maxR = radiusCrds.x;
                minR = radiusCrds.y;
                swap = true;
            }
            uint_fast32_t numVertices = getNumEllipseVertices(radiusCrds);
            ColorVertex *memoryCol = NULL;
            TexturedColorVertex *memoryTextured = NULL;
            if(textured){
                memoryTextured = static_cast<TexturedColorVertex *>(memoryVoid);
            } else {
                memoryCol = static_cast<ColorVertex *>(memoryVoid);
            }
            for(uint_fast32_t i = 0; i <= minR; i++) {
                uint_fast32_t cur = (swap) ? (minR - i) : i;
                double first;
                double second;
                if (swap) {
                    second = static_cast<float>(cur);
                    first = maxR * sqrt(1 - (second / minR) * (second / minR));
                } else {
                    first = static_cast<float>(cur);
                    second = maxR * sqrt(1 - (first / minR) * (first / minR));
                }
                if (i == 0) {
                    for (uint_fast32_t j = 0; j < (numVertices - 1); j += 4) {
                        if(textured){
                            memoryTextured[j] = createTexturedVertex(
                                    centerCrds.x + first,
                                    centerCrds.y - second 
                            );
                        } else {
                            memoryCol[j] = createVertex(
                                    centerCrds.x + first ,
                                    centerCrds.y - second 
                            );
                        }
                    }
                    if(textured){
                        memoryTextured[numVertices - 1] = createTexturedVertex(
                                centerCrds.x + first,
                                centerCrds.y - second 
                        );
                    } else {
                        memoryCol[numVertices - 1] = createVertex(
                                centerCrds.x + first,
                                centerCrds.y - second 
                        );
                    }
                    uint_fast32_t oppIndex = 2 * minR;
                    createVertices(memoryVoid, centerCrds.x + first, centerCrds.y + second, oppIndex, textured);
                } else if (i == minR) {
                    uint_fast32_t curIndex = minR;
                    uint_fast32_t oppIndex = 3 * minR;
                    createVertices(memoryVoid, centerCrds.x + first + EXTRA_OFFSET, centerCrds.y - second, curIndex, textured);
                    createVertices(memoryVoid, centerCrds.x - first, centerCrds.y + second, oppIndex, textured);
                } else {
                    for (uint_fast32_t j = 0; j < 4; j++) {
                        uint_fast32_t index;
                        double x, y;
                        if (j == 0) {
                            index = i;
                            x = centerCrds.x + first + EXTRA_OFFSET;
                            y = centerCrds.y - second;
                        } else if (j == 1) {
                            index = 2 * minR - i;
                            x = centerCrds.x + first + EXTRA_OFFSET;
                            y = centerCrds.y + second;
                        } else if (j == 2) {
                            index = 2 * minR + i;
                            x = centerCrds.x - first;
                            y = centerCrds.y + second;
                        } else {
                            index = 4 * minR - i;
                            x = centerCrds.x - first;
                            y = centerCrds.y - second;
                        }
                        createVertices(memoryVoid, x, y, index, textured);
                    }
                }
            }
            if(textured){
                return memoryTextured + numVertices;
            } else {
                return memoryCol + numVertices;
            }
        }

        uint_fast32_t EllipseHelper::getNumEllipseVertices(
                const UCoords & radiusCrds
        ) {
            uint_fast32_t numPoints = std::min(radiusCrds.x, radiusCrds.y);
            return (numPoints * 4 - 1) / 2 * 4 + 3;
        }
        ColorVertex EllipseHelper::createVertex(
                double x, double y
        ) {
            return VertexCreator::create<ColorVertex>(
                    static_cast<float>(x - 0.5), static_cast<float>(y - 0.5), _z, 1.0f, _color
            );
        }

        TexturedColorVertex EllipseHelper::createTexturedVertex(double x, double y) {
            return VertexCreator::create<TexturedColorVertex>(
                    static_cast<float>(x - 0.5), static_cast<float>(y - 0.5), _z, 1.0f, _color,
                    static_cast<float>((x - _minX) / FPATTERN_SIZE),
                    static_cast<float>((y - _minY) / FPATTERN_SIZE)
            );
        }
    }
}

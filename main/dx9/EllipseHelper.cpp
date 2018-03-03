#include <algorithm>
#include "EllipseHelper.h"
#include "util.h"

#undef min

namespace directgraph{
    namespace dx9{
        const float EllipseHelper::EXTRA_OFFSET = 0.005f;

        EllipseHelper::EllipseHelper() {

        }

        ColorVertex *EllipseHelper::genEllipse(
                void *memoryVoid,
                int_fast32_t centerX, int_fast32_t centerY,
                uint_fast32_t rx, uint_fast32_t ry,
                uint_fast32_t color
        ) {
            color = swap_color(color);
            bool swap;
            uint_fast32_t minR, maxR;
            if(ry > rx){
                maxR = ry;
                minR = rx;
                swap = false;
            } else {
                maxR = rx;
                minR = ry;
                swap = true;
            }
            uint_fast32_t numVertices = getNumEllipseVertices(rx, ry);
            ColorVertex *memory = static_cast<ColorVertex *>(memoryVoid);
            for(uint_fast32_t i = 0; i <= minR; i++) {
                uint_fast32_t cur = (swap) ? (minR - i) : i;
                float first;
                float second;
                if (swap) {
                    second = static_cast<float>(cur);
                    first = maxR * sqrt(1 - (second / minR) * (second / minR));
                } else {
                    first = static_cast<float>(cur);
                    second = maxR * sqrt(1 - (first / minR) * (first / minR));
                }
                if (i == 0) {
                    for (uint_fast32_t j = 0; j < (numVertices - 1); j += 3) {
                        memory[j] = VertexCreator::create<ColorVertex>(
                                centerX + first - 0.5f,
                                centerY - second - 0.5f,
                                0.0f,
                                1.0f,
                                color
                        );
                    }
                    memory[numVertices - 1] = VertexCreator::create<ColorVertex>(
                            centerX + first - 0.5f,
                            centerY - second - 0.5f,
                            0.0f,
                            1.0f,
                            color
                    );
                    uint_fast32_t oppIndex = convertIndex(2 * minR);
                    memory[oppIndex] = VertexCreator::create<ColorVertex>(
                            centerX + first - 0.5f,
                            centerY + second - 0.5f,
                            0.0f,
                            1.0f,
                            color
                    );
                } else if (i == minR) {
                    uint_fast32_t curIndex = convertIndex(minR);
                    memory[curIndex] = VertexCreator::create<ColorVertex>(
                            centerX + first - 0.5f + EXTRA_OFFSET,
                            centerY - second - 0.5f,
                            0.0f,
                            1.0f,
                            color
                    );
                    uint_fast32_t oppIndex = convertIndex(3 * minR);
                    memory[oppIndex] = VertexCreator::create<ColorVertex>(
                            centerX - first - 0.5f,
                            centerY + second - 0.5f,
                            0.0f,
                            1.0f,
                            color
                    );
                } else {
                    for (uint_fast32_t j = 0; j < 4; j++) {
                        uint_fast32_t index;
                        float x, y;
                        if (j == 0) {
                            index = convertIndex(i);
                            x = centerX + first - 0.5f + EXTRA_OFFSET;
                            y = centerY - second - 0.5f;
                        } else if (j == 1) {
                            index = convertIndex(2 * minR - i);
                            x = centerX + first - 0.5f + EXTRA_OFFSET;
                            y = centerY + second - 0.5f;
                        } else if (j == 2) {
                            index = convertIndex(2 * minR + i);
                            x = centerX - first - 0.5f;
                            y = centerY + second - 0.5f;
                        } else {
                            index = convertIndex(4 * minR - i);
                            x = centerX - first - 0.5f;
                            y = centerY - second - 0.5f;
                        }
                        memory[index] = VertexCreator::create<ColorVertex>(
                                x, y,
                                0.0f, 1.0f, color
                        );
                    }
                }
            }
            return memory + numVertices;
        }

        uint_fast32_t EllipseHelper::getNumEllipseVertices(
                uint_fast32_t rx, uint_fast32_t ry
        ) {
            uint_fast32_t numPoints = std::min(rx, ry) * 4;
            return (numPoints - 1) / 2 + numPoints + 1;
        }

        uint_fast32_t EllipseHelper::convertIndex(uint_fast32_t index) {
            uint_fast32_t t = (1 - index % 2);
            return 3 * ((index - t) / 2) + t + 1;
        }
    }
}

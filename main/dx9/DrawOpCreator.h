#pragma once

#include <main/IPixelContainer.h>
#include <vector>

namespace directgraph {
    namespace dx9 {
        namespace DrawOpType {
            enum Type {
                START_TRANSPARENT_DATA,
                REMOVE_PIXEL_TEXTURE,
                REMOVE_PATTERN_TEXTURE,
                SET_FILL_PATTERN,
                SET_FILL_PATTERN_COLOR,
                SET_FILL_PATTERN_TWO_COLORS,
                SET_USER_FILL_PATTERN,
                SET_TEX_BG_COLOR,
                ITEMS,
                SET_PIXEL_TEXTURE,
                SET_LINE_PATTERN
            };
        }
        namespace DrawDataType {
            enum Type {
                COLOR_VERTEX,
                TEXTURED_COLOR_VERTEX,
                TEXTURED_VERTEX,
                COLOR2_VERTEX,
                ELLIPSE_VERTEX,
                TEXTURED_ELLIPSE_VERTEX
            };
        }
        struct DrawOp{
            DrawOpType::Type type;
            union {
                struct FillPatternColor {
                    uint8_t fillPattern;
                    uint32_t bgColor;
                } fillPatternColor;
                struct FillPatternTwoColors {
                    uint8_t fillPattern;
                    uint32_t bgColor;
                    uint32_t fillColor;
                } fillPatternTwoColors;
                uint8_t fillPattern;
                uint8_t linePattern;
                uint32_t bgColor;
                char *userFillPattern;
                struct Items{
                    uint32_t size;
                    uint32_t numItems;
                    DrawDataType::Type type;
                } items;
                IPixelContainer *pixelContainer;
            } data;
        };

        typedef std::vector<DrawOp> DrawOpVector;

        class DrawOpCreator{
        public:
            template <uint_fast32_t T>
            static DrawOp create();
            template <uint_fast32_t T>
            static DrawOp create(uint_fast32_t);
            template <uint_fast32_t T>
            static DrawOp create(uint_fast32_t, uint_fast32_t);
            template <uint_fast32_t T>
            static DrawOp create(uint_fast32_t, uint_fast32_t, uint_fast32_t);
            template <uint_fast32_t T>
            static DrawOp create(char *userFillPattern);
            template <uint_fast32_t T>
            static DrawOp create(
                    uint32_t size, uint32_t numItems, DrawDataType::Type type
            );
            template <uint_fast32_t T>
            static DrawOp create(
                    IPixelContainer *container
            );
        };
    }
}



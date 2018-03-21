#include "DrawOpCreator.h"

namespace directgraph{
    namespace dx9{
        template<>
        DrawOp DrawOpCreator::create<DrawOpType::REMOVE_PIXEL_TEXTURE>() {
            DrawOp op;
            op.type = DrawOpType::REMOVE_PIXEL_TEXTURE;
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::START_TRANSPARENT_DATA>() {
            DrawOp op;
            op.type = DrawOpType::START_TRANSPARENT_DATA;
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::REMOVE_PATTERN_TEXTURE>() {
            DrawOp op;
            op.type = DrawOpType::REMOVE_PATTERN_TEXTURE;
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::SET_FILL_PATTERN>(uint_fast32_t fillPattern) {
            DrawOp op;
            op.type = DrawOpType::SET_FILL_PATTERN;
            op.data.fillPattern = static_cast<uint8_t>(fillPattern);
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::SET_LINE_PATTERN>(uint_fast32_t linePattern) {
            DrawOp op;
            op.type = DrawOpType::SET_LINE_PATTERN;
            op.data.linePattern = static_cast<uint8_t>(linePattern);
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::SET_FILL_PATTERN_COLOR>(
                uint_fast32_t fillPattern, uint_fast32_t bgColor
        ) {
            DrawOp op;
            op.type = DrawOpType::SET_FILL_PATTERN_COLOR;
            op.data.fillPatternColor.fillPattern = static_cast<uint8_t>(fillPattern);
            op.data.fillPatternColor.bgColor = bgColor;
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::SET_FILL_PATTERN_TWO_COLORS>(
                uint_fast32_t fillPattern, uint_fast32_t bgColor, uint_fast32_t fillColor
        ) {
            DrawOp op;
            op.type = DrawOpType::SET_FILL_PATTERN_TWO_COLORS;
            op.data.fillPatternTwoColors.fillPattern = static_cast<uint8_t>(fillPattern);
            op.data.fillPatternTwoColors.bgColor = bgColor;
            op.data.fillPatternTwoColors.fillColor = fillColor;
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::SET_TEX_BG_COLOR>(uint32_t bgColor) {
            DrawOp op;
            op.type = DrawOpType::SET_TEX_BG_COLOR;
            op.data.bgColor = bgColor;
            return op;
        }

        template<>
        DrawOp
        DrawOpCreator::create<DrawOpType::ITEMS>(
                uint32_t size, uint32_t numItems, DrawDataType::Type type
        ) {
            DrawOp op;
            op.type = DrawOpType::ITEMS;
            op.data.items.size = size;
            op.data.items.numItems = numItems;
            op.data.items.type = type;
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::SET_PIXEL_TEXTURE>(
                IPixelContainer *container
        ) {
            DrawOp op;
            op.type = DrawOpType::SET_PIXEL_TEXTURE;
            op.data.pixelContainer = container;
            return op;
        }

        template<>
        DrawOp DrawOpCreator::create<DrawOpType::SET_USER_FILL_PATTERN>(char *userFillPattern) {
            DrawOp op;
            op.type = DrawOpType::SET_USER_FILL_PATTERN;
            op.data.userFillPattern = userFillPattern;
            return op;
        }
    }
}

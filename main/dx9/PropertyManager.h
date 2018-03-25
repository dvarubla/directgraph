#pragma once

#include <stdint.h>
#include <array>

namespace directgraph {
    namespace dx9 {
        struct RenderProperty{
            bool isSet;
            union {
                uint_fast32_t val;
                void *valP;
            };
        };

        namespace PropertyName {
            enum Name {
                FILL_PATTERN = 0,
                USER_FILL_PATTERN,
                USER_LINE_PATTERN,
                LINE_PATTERN,
                FILL_COLOR,
                DRAW_COLOR,
                BG_COLOR,
                SHADER_TYPE,
                TEXTURE_STATE,
                PIXEL_CONTAINER,
                TOTAL_PROP_COUNT
            };
        }

        namespace ShaderType {
            enum Type {
                ELLIPSE_SHADER,
                TEXTURED_ELLIPSE_SHADER,
                TEXTURED_BAR_SHADER,
                TEXTURED_RECTANGLE_SHADER
            };
        }

        namespace TextureState {
            enum State {
                FILL_TEXTURE,
                LINE_TEXTURE
            };
        }

        typedef std::array<RenderProperty, PropertyName::TOTAL_PROP_COUNT> ItemState;

        struct ItemStateHash {
            std::size_t operator()(const ItemState &state) const;
        };

        bool operator==(const ItemState &l, const ItemState &r);

        struct ItemStateDiffPart{
            bool added;
            bool changed;
            bool removed;
        };

        typedef std::array<ItemStateDiffPart, PropertyName::TOTAL_PROP_COUNT> ItemStateDiff;

        class PropertyManager {
        public:
            PropertyManager();
            ItemStateDiff getItemStateDiff(const ItemState &statePrev, const ItemState &stateCurr);
            ItemState getInitialItemState();
            ItemState getNullState();
            void setProp(ItemState &state, PropertyName::Name name, uint_fast32_t val);
            void setPropP(ItemState &state, PropertyName::Name name, void* valP);
        };
    }
}


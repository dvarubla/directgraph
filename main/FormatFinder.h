#pragma once

#include <util.h>

namespace directgraph {
    namespace ColorFormat{
        enum Format{
            A8R8G8B8,
            AR5G5B5,
            A8,
            A4R4G4B4
        };
    }

    template <uint_fast32_t DispMode>
    class FormatFinder{
    };

    template <>
    class FormatFinder<ColorFormat::A8R8G8B8>{
    public:
        typedef uint32_t FormatType;
        static FormatType convert(uint32_t color, bool isTransparent){
            if(isTransparent){
                return swap_color(color);
            } else {
                return color_has_alpha(color) ? swap_color_transp(color) : (swap_color(color) | 0xFF000000);
            }
        }
    };

    template <>
    class FormatFinder<ColorFormat::A4R4G4B4>{
    public:
        typedef uint16_t FormatType;
        static FormatType convert(uint32_t color, bool isTransparent){
            uint16_t mainColor = static_cast<uint16_t>((((color & 0xFF) * 0xF / 0xFF) << 8) |
                                                       ((((color >> 8) & 0xFF) * 0xF / 0xFF) << 4) |
                                                       (((color >> 16) & 0xFF) * 0xF / 0xFF));
            uint16_t alpha = static_cast<uint16_t>(((color >> 24) & 0xFF) * 0xF / 0xFF);
            if(isTransparent) {
                return mainColor;
            } else {
                if(alpha != 0) {
                    return static_cast<uint16_t>(mainColor | ((0xFF - alpha) << 12));
                } else {
                    return static_cast<uint16_t>(mainColor | 0xF000);
                }
            }
        }
    };

    template <>
    class FormatFinder<ColorFormat::AR5G5B5>{
    public:
        typedef uint16_t FormatType;
        static FormatType convert(uint32_t color, bool isTransparent){
            return
                    static_cast<uint16_t>
                    (
                     (((color & 0xFF) * 0x1F / 0xFF) << 10) |
                     ((((color >> 8) & 0xFF) * 0x1F / 0xFF) << 5) |
                     (((color >> 16) & 0xFF) * 0x1F / 0xFF) |
                     ((isTransparent) ? 0 : (1 << 15))
                    )
                    ;
        }
    };

    template <>
    class FormatFinder<ColorFormat::A8>{
    public:
        typedef uint8_t FormatType;
        static FormatType convert(uint32_t, bool isTransparent){
            return convert(isTransparent);
        }

        static FormatType convert(bool isTransparent){
            return static_cast<FormatType>((isTransparent) ? 0 : 0xFF);
        }
    };
}



#pragma once

#include <util.h>

namespace directgraph {
    namespace ColorFormat{
        enum Format{
            A8R8G8B8,
            AR5G5B5,
            A8
        };
    };

    template <uint_fast32_t DispMode>
    class FormatFinder{
    };

    template <>
    class FormatFinder<ColorFormat::A8R8G8B8>{
    public:
        typedef uint32_t FormatType;
        static FormatType convert(uint32_t color, bool isTransparent){
            return ((isTransparent) ? 0 : 0xFF000000) | swap_color(color);
        }
    };

    template <>
    class FormatFinder<ColorFormat::AR5G5B5>{
    public:
        typedef uint16_t FormatType;
        static FormatType convert(uint32_t color, bool isTransparent){
            return
                    static_cast<uint16_t>
                    ((((color & 0xFF) / 8) << 10) |
                     ((((color >> 8) & 0xFF) / 8) << 5) |
                     ((((color >> 16) & 0xFF)) / 8)) |
                     static_cast<FormatType>((isTransparent) ? 0 : (1 << 15))
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



#pragma once

#include <util.h>

namespace directgraph {
    namespace ColorFormat{
        enum Format{
            R5G6B5,
            AR5G5B5,
            R8G8B8,
            A8
        };
    };

    template <uint_fast32_t DispMode>
    class FormatFinder{
    };

    template <>
    class FormatFinder<ColorFormat::Format::R8G8B8>{
    public:
        typedef uint32_t FormatType;
        static FormatType convert(uint32_t color){
            return swap_color(color);
        }
    };

    template <>
    class FormatFinder<ColorFormat::R5G6B5>{
    public:
        typedef uint16_t FormatType;
        static FormatType convert(uint32_t color){
            return
                    static_cast<uint16_t>
                    ((((color & 0xFF) / 8) << 11) |
                     ((((color >> 8) & 0xFF) / 8) << 6) |
                     ((((color >> 16) & 0xFF)) / 8))
                    ;
        }
    };

    template <>
    class FormatFinder<ColorFormat::Format::AR5G5B5>{
    public:
        typedef uint16_t FormatType;
        static FormatType convert(uint32_t color, bool isTransparent){
            return
                    static_cast<uint16_t>
                    ((((color & 0xFF) / 8) << 10) |
                     ((((color >> 8) & 0xFF) / 8) << 5) |
                     ((((color >> 16) & 0xFF)) / 8)) |
                     static_cast<FormatType>((isTransparent) ? (1 << 15) : 0)
                    ;
        }
    };

    template <>
    class FormatFinder<ColorFormat::Format::A8>{
    public:
        typedef uint8_t FormatType;
        static FormatType convert(uint32_t, bool isTransparent){
            return convert(isTransparent);
        }

        static FormatType convert(bool isTransparent){
            return static_cast<FormatType>((isTransparent) ? 0xFF : 0);
        }
    };
}



#pragma once

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
    class FormatFinder<ColorFormat::R8G8B8>{
    public:
        typedef uint32_t ContainerType;
        static ContainerType convert(uint32_t color){
            return swap_color(color);
        }
    };

    template <>
    class FormatFinder<ColorFormat::R5G6B5>{
    public:
        typedef uint16_t ContainerType;
        static ContainerType convert(uint32_t color){
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
        typedef uint16_t ContainerType;
        static ContainerType convert(uint32_t color, bool isTransparent){
            return
                    static_cast<uint16_t>
                    ((((color & 0xFF) / 8) << 10) |
                     ((((color >> 8) & 0xFF) / 8) << 5) |
                     ((((color >> 16) & 0xFF)) / 8)) |
                     static_cast<ContainerType>((isTransparent) ? (1 << 11) : 0)
                    ;
        }
    };

    template <>
    class FormatFinder<ColorFormat::Format::A8>{
    public:
        typedef uint8_t ContainerType;
        static ContainerType convert(bool isTransparent){
            return static_cast<ContainerType>((isTransparent) ? 0xFF : 0);
        }
    };
}



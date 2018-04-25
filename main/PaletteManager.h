#pragma once

#include <stdint.h>
#include <vector>

namespace directgraph {
    class PaletteManager {
    private:
        std::vector<uint_fast32_t> _colors;
        bool _havePalette;
        void checkIndex(uint_fast32_t index);
    public:
        PaletteManager();
        void initPalette(bool havePalette, uint_fast32_t size, bool fillFirstColors);
        void clearPalette();
        uint_fast32_t getColor(uint_fast32_t indexOrColor);
        void setColor(uint_fast32_t index, uint_fast32_t color);
    };
}



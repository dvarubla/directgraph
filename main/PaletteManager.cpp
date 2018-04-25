#include "PaletteManager.h"
#include "WException.h"
#include "util.h"
#include <windows.h>

namespace directgraph{

    PaletteManager::PaletteManager(): _havePalette(false) {

    }

    void PaletteManager::initPalette(bool havePalette, uint_fast32_t size, bool fillFirstColors) {
        _havePalette = havePalette;
        if(_havePalette){
            _colors.resize(size, 0);
            if(fillFirstColors){
                clearPalette();
            }
        } else {
            _colors.clear();
        }
    }

    uint_fast32_t PaletteManager::getColor(uint_fast32_t indexOrColor) {
        if(_havePalette) {
            checkIndex(indexOrColor);
            return _colors[indexOrColor];
        } else {
            return indexOrColor;
        }
    }

    void PaletteManager::setColor(uint_fast32_t index, uint_fast32_t color) {
        if(_havePalette) {
            checkIndex(index);
            _colors[index] = color;
        } else {
            THROW_EXC_CODE(WException, WRONG_CALL, L"You can't set color when there is no palette");
        }
    }

    void PaletteManager::clearPalette() {
        if(_colors.size() == 16){
            _colors[0] = RGB(  0,  0,  0);
            _colors[1] = RGB(  0, 0, 255);
            _colors[2] = RGB(  0,255,  0);
            _colors[3] = RGB(  0,255,255);
            _colors[4] = RGB(255,  0,  0);
            _colors[5] = RGB(255,  0,255);
            _colors[6] = RGB(150, 75,  0);
            _colors[7] = RGB(187,187,187);
            _colors[8] = RGB(128,128,128);
            _colors[9] = RGB( 66,170,255);
            _colors[10] = RGB(144,238,144);
            _colors[11] = RGB(224,255,255);
            _colors[12] = RGB(255,153,153);
            _colors[13] = RGB(255,119,255);
            _colors[14] = RGB(255,255,  0);
            _colors[15] = RGB(255,255,255);
        }
    }

    void PaletteManager::checkIndex(uint_fast32_t index) {
        if(index >= _colors.size()){
            THROW_EXC_CODE(
                    WException, WRONG_DRAW_PARAM,
                    L"Index " + to_wstring(index) + L" is >= than palette size " + to_wstring(_colors.size())
            );
        }
    }
}
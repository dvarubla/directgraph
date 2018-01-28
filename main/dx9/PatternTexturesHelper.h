#pragma once

#include <cstdint>
#include <directgraph/directgraph_constants.h>
#include <d3d9.h>
#include <main/FormatFinder.h>
#include <patterns.h>
#include "IPatternTexturesHelper.h"

namespace directgraph {
    namespace dx9 {
        template<uint_fast32_t DispMode>
        class PatternTexturesHelper: public IPatternTexturesHelper {
        private:
            typedef FormatFinder<DispMode> FrmFinder;
            typedef typename FrmFinder::FormatType TextureType;

            struct TextureBlock{
                IDirect3DTexture9 *texture;
                uint_fast32_t color;
            };
            const static int NUM_TOTAL_FPATTERNS = NUM_STANDARD_FPATTERNS + 1;
            const static int USER_PATTERN_INDEX = NUM_TOTAL_FPATTERNS - 1;
            const static UINT FPATTERN_W = 8;
            const static UINT FPATTERN_H = FPATTERN_SIZE;

            TextureBlock _textures[NUM_TOTAL_FPATTERNS];
            IDirect3DDevice9 *_device;
            D3DFORMAT _textureFormat;
            bool _haveConstantSupport;
            uint8_t *_userPattern;
            bool _haveUserPattern;
            bool _needCreateUserPattern;

            void createFPattern(uint_fast8_t index, uint_fast32_t bgColor, const char *pattern, bool forceCreate);
            void setTexture(uint_fast8_t index, uint_fast32_t bgColor);
        public:
            PatternTexturesHelper(IDirect3DDevice9 *device, D3DFORMAT textureFormat, bool haveConstantSupport);
            ~PatternTexturesHelper();
            void setFillPattern(uint_fast8_t pattern, uint_fast32_t bgColor);
            void setUserFillPattern(const char *pattern);
            void unsetPattern();
        };
    }
}



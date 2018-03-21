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

            struct FillTextureBlock{
                IDirect3DTexture9 *texture;
                uint_fast32_t bgColor;
                uint_fast32_t fillColor;
            };
            struct LineTextureBlock{
                IDirect3DTexture9 *texture;
                uint_fast32_t color;
            };
            const static int NUM_TOTAL_FPATTERNS = NUM_STANDARD_FPATTERNS + 1;
            const static int USER_FPATTERN_INDEX = NUM_TOTAL_FPATTERNS - 1;
            const static UINT FPATTERN_W = 8;
            const static UINT FPATTERN_H = FPATTERN_SIZE;

            const static int NUM_TOTAL_LPATTERNS = NUM_STANDARD_LPATTERNS + 1;
            const static int USER_LPATTERN_INDEX = NUM_TOTAL_LPATTERNS - 1;
            const static UINT LPATTERN_W = 16;
            const static UINT LPATTERN_H = 16;

            FillTextureBlock _fTextures[NUM_TOTAL_FPATTERNS];
            LineTextureBlock _lTextures[NUM_TOTAL_LPATTERNS];
            IDirect3DDevice9 *_device;
            D3DFORMAT _textureFormat;
            bool _haveConstantSupport;
            uint8_t *_userFPattern;
            bool _haveUserFPattern;
            uint16_t _userLPattern;
            bool _haveUserLPattern;
            bool _needCreateUserPattern;

            void createFPattern(
                    uint_fast8_t index,
                    uint_fast32_t bgColor,
                    uint_fast32_t fillColor,
                    const char *pattern,
                    bool forceCreate,
                    bool needRecreate,
                    bool useTransparency
            );
            void setFTexture(uint_fast8_t index, bool useBgColor, bool useFgColor, bool useTransparency);
            void setBgColorConstant(uint_fast32_t bgColor);
            uint_fast8_t setFillPattern(
                    uint_fast8_t pattern,
                    uint_fast32_t bgColor,
                    uint_fast32_t fillColor,
                    bool needRecreate, bool useTransparency
            );

            void createLPattern(
                    uint_fast8_t index,
                    uint_fast32_t color,
                    uint_fast16_t pattern,
                    bool forceCreate,
                    bool needRecreate
            );
            uint_fast8_t setLinePattern(
                    uint_fast8_t pattern,
                    uint_fast32_t color,
                    bool needRecreate
            );
            void setLTexture(uint_fast8_t index, bool useColor, bool useTransparency);

        public:
            PatternTexturesHelper(IDirect3DDevice9 *device, D3DFORMAT textureFormat, bool haveConstantSupport);
            ~PatternTexturesHelper();
            void setUserFillPattern(const char *pattern);
            virtual void setBgColor(uint_fast32_t bgColor);
            virtual void setFillPattern(uint_fast8_t pattern, bool useTransparency);
            virtual void setLinePattern(uint_fast8_t pattern, bool useTransparency);
            virtual void setFillPatternBgFillColor(uint_fast8_t pattern, uint_fast32_t bgColor, uint_fast32_t fillColor);
            virtual void setFillPatternBgColor(uint_fast8_t pattern, uint_fast32_t bgColor, bool useTransparency);
            void unsetPattern(bool useTransparency);
        };
    }
}



#include <main/WException.h>
#include "PatternTexturesHelper.h"
#include "Exception.h"

namespace directgraph{
    namespace dx9{
        template<uint_fast32_t DispMode>
        PatternTexturesHelper<DispMode>::PatternTexturesHelper(
                IDirect3DDevice9 *device, D3DFORMAT textureFormat, bool haveConstantSupport
        ): _device(device), _textureFormat(textureFormat),
           _haveConstantSupport(haveConstantSupport),
           _userFPattern(new uint8_t[FPATTERN_SIZE]), _haveUserFPattern(false), _haveUserLPattern(false) {
            for(uint_fast8_t i = 0; i < NUM_TOTAL_FPATTERNS; i++){
                _fTextures[i].texture = NULL;
            }
            for(uint_fast8_t i = 0; i < NUM_TOTAL_LPATTERNS; i++){
                _lTextures[i].texture = NULL;
            }
            if(haveConstantSupport) {
                for(uint_fast8_t i = 0; i < NUM_STANDARD_FPATTERNS; i++) {
                    createFPattern(i, 0, 0, reinterpret_cast<const char *>(fillPatterns[i]), false, true, false);
                }
                for(uint_fast8_t i = 0; i < NUM_STANDARD_LPATTERNS; i++) {
                    createLPattern(i, 0, linePatterns[i], false, false);
                }
            }
        }

        template<uint_fast32_t DispMode>
        uint_fast8_t PatternTexturesHelper<DispMode>::setFillPattern(
                uint_fast8_t pattern, uint_fast32_t bgColor, uint_fast32_t fillColor,
                bool needRecreate, bool useTransparency
        ) {
            bool setStandardPattern = true;
            if(pattern == USER_FILL){
                if(!_haveUserFPattern){
                    pattern = CLOSE_DOT_FILL;
                } else {
                    setStandardPattern = false;
                    pattern = USER_FPATTERN_INDEX;
                    createFPattern(
                            USER_FPATTERN_INDEX, bgColor, fillColor,
                            reinterpret_cast<const char *>(_userFPattern),
                            _needCreateUserPattern,
                            needRecreate,
                            useTransparency
                    );
                    _needCreateUserPattern = false;
                }
            }
            if(setStandardPattern){
                pattern -= FIRST_FPATTERN;
                if(needRecreate) {
                    createFPattern(
                            pattern, bgColor, fillColor,
                            reinterpret_cast<const char *>(fillPatterns[pattern]),
                            false,
                            true,
                            useTransparency
                    );
                }
            }
            return pattern;
        }

        template<uint_fast32_t DispMode>
        uint_fast8_t PatternTexturesHelper<DispMode>::setLinePattern(
                uint_fast8_t pattern, uint_fast32_t color, bool needRecreate) {
            pattern -= FIRST_LPATTERN;
            if(needRecreate) {
                createLPattern(
                        pattern, color,
                        linePatterns[pattern],
                        false,
                        true
                );
            }
            return pattern;
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setLinePattern(uint_fast8_t pattern, bool useTransparency) {
            pattern = setLinePattern(pattern, 0, false);
            setLTexture(pattern, false, useTransparency);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFillPattern(uint_fast8_t pattern, bool useTransparency) {
            pattern = setFillPattern(pattern, 0, 0, false, useTransparency);
            setFTexture(pattern, false, false, useTransparency);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFillPatternBgColor(
                uint_fast8_t pattern, uint_fast32_t bgColor, bool useTransparency
        ) {
            pattern = setFillPattern(pattern, bgColor, bgColor, true, useTransparency);
            setFTexture(pattern, true, false, useTransparency);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFillPatternBgFillColor(
                uint_fast8_t pattern, uint_fast32_t bgColor, uint_fast32_t fillColor) {
            pattern = setFillPattern(pattern, bgColor, fillColor, true, true);
            setFTexture(pattern, true, true, true);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setBgColorConstant(uint_fast32_t bgColor) {
            _device->SetTextureStageState(0, D3DTSS_CONSTANT, swap_color_transp(bgColor));
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setBgColor(uint_fast32_t bgColor) {
            setBgColorConstant(bgColor);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setUserFillPattern(const char *pattern) {
            std::copy(pattern, pattern + FPATTERN_SIZE, _userFPattern);
            _haveUserFPattern = true;
            _needCreateUserPattern = true;
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::unsetPattern(bool useTransparency) {
            _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            _device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
            _device->SetTexture(0, NULL);
            if(useTransparency){
                _device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
                _device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
            }
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::createFPattern(
                uint_fast8_t index, uint_fast32_t bgColor, uint_fast32_t fillColor, const char *pattern,
                bool forceCreate, bool needRecreate, bool useTransparency
        ) {
            bool needCreate = false;
            if(_fTextures[index].texture == NULL) {
                needCreate = true;
                if (_device->CreateTexture(
                        FPATTERN_W, FPATTERN_H, 1, 0,
                        _textureFormat, D3DPOOL_MANAGED, &_fTextures[index].texture, NULL
                ) != D3D_OK) {
                    THROW_EXC_CODE(Exception, DX9_CANT_CREATE_TEXTURE, std::wstring(L"Can't create pattern texture"));
                };
            }
            if(
                    needCreate || forceCreate ||
                    (needRecreate && _fTextures[index].bgColor != bgColor) ||
                    (needRecreate && useTransparency && _fTextures[index].fillColor != fillColor)
            ) {
                D3DLOCKED_RECT outRect;
                _fTextures[index].texture->LockRect(0, &outRect, NULL, 0);
                int rowPitch = outRect.Pitch;
                TextureType * pixels = (TextureType *)outRect.pBits;
                for (uint_fast8_t i = 0; i < FPATTERN_H; i++) {
                    for (uint_fast8_t j = 0; j < FPATTERN_W; j++) {
                        bool isTransparent = (static_cast<uint8_t>(pattern[i]) & (1 << (FPATTERN_W - j - 1))) ==
                                             (1 << (FPATTERN_W - j - 1));
                        pixels[j] = FrmFinder::convert(
                                (isTransparent) ? fillColor : bgColor,
                                isTransparent && !(needRecreate && useTransparency)
                        );
                    }
                    pixels += rowPitch / sizeof(TextureType);
                }
                _fTextures[index].texture->UnlockRect(0);
                _fTextures[index].bgColor = bgColor;
                _fTextures[index].fillColor = fillColor;
            }
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::createLPattern(
                uint_fast8_t index, uint_fast32_t color, uint_fast16_t pattern,
                bool forceCreate, bool needRecreate
        ) {
            bool needCreate = false;
            if(_lTextures[index].texture == NULL) {
                needCreate = true;
                if (_device->CreateTexture(
                        LPATTERN_W, LPATTERN_H, 1, 0,
                        _textureFormat, D3DPOOL_MANAGED, &_lTextures[index].texture, NULL
                ) != D3D_OK) {
                    THROW_EXC_CODE(Exception, DX9_CANT_CREATE_TEXTURE, std::wstring(L"Can't create pattern texture"));
                };
            }
            if(
                    needCreate || forceCreate ||
                    (needRecreate && _lTextures[index].color != color)
            ) {
                D3DLOCKED_RECT outRect;
                _lTextures[index].texture->LockRect(0, &outRect, NULL, 0);
                int rowPitch = outRect.Pitch;
                TextureType * pixels = (TextureType *)outRect.pBits;
                for (uint_fast8_t i = 0; i < LPATTERN_H; i++) {
                    for (uint_fast8_t j = 0; j < LPATTERN_W; j++) {
                        bool isTransparent = ((pattern & (1 << j)) == (1 << j));
                        pixels[j] = FrmFinder::convert(
                                (isTransparent) ? 0xFF000000 : color,
                                isTransparent && !needRecreate
                        );
                    }
                    pixels += rowPitch / sizeof(TextureType);
                }
                _lTextures[index].texture->UnlockRect(0);
                _lTextures[index].color = color;
            }
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFTexture(
                uint_fast8_t index, bool useBgColor, bool useFgColor, bool useTransparency
        ) {
            if(index >= NUM_TOTAL_FPATTERNS){
                THROW_EXC_CODE(WException, UNREACHABLE_CODE, std::wstring(L"Bad pattern index"));
            }
            _device->SetTexture(0, _fTextures[index].texture);
            _device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            _device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            _device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            if(useFgColor){
                _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                _device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
            } else {
                if (useBgColor) {
                    _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                } else if(_haveConstantSupport){
                    _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CONSTANT);
                }
                _device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
                if (_haveConstantSupport && useTransparency) {
                    _device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);
                    _device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
                    _device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_BLENDTEXTUREALPHA);
                }
            }
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setLTexture(uint_fast8_t index, bool useColor, bool useTransparency) {
            _device->SetTexture(0, _lTextures[index].texture);
            _device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            _device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            if(useColor){
                _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                _device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
            } else {
                _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
                _device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
                if(useTransparency){
                    _device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
                    _device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
                    _device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SUBTRACT);
                }
            }
        }

        template<uint_fast32_t DispMode>
        PatternTexturesHelper<DispMode>::~PatternTexturesHelper() {
            _device->SetTexture(0, NULL);
            for(uint_fast8_t i = 0; i < NUM_TOTAL_FPATTERNS; i++){
                if(_fTextures[i].texture != NULL){
                    _fTextures[i].texture->Release();
                }
            }
            delete [] _userFPattern;
            for(uint_fast8_t i = 0; i < NUM_TOTAL_LPATTERNS; i++){
                if(_lTextures[i].texture != NULL){
                    _lTextures[i].texture->Release();
                }
            }
        }

        template class PatternTexturesHelper<ColorFormat::A8R8G8B8>;
        template class PatternTexturesHelper<ColorFormat::A8>;
        template class PatternTexturesHelper<ColorFormat::AR5G5B5>;
        template class PatternTexturesHelper<ColorFormat::A4R4G4B4>;
    }
}

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
           _userPattern(new uint8_t[FPATTERN_SIZE]), _haveUserPattern(false) {
            for(uint_fast8_t i = 0; i < NUM_TOTAL_FPATTERNS; i++){
                _textures[i].texture = NULL;
            }
            if(haveConstantSupport) {
                for(uint_fast8_t i = 0; i < NUM_STANDARD_FPATTERNS; i++) {
                    createFPattern(i, 0, 0, reinterpret_cast<const char *>(fillPatterns[i]), false, true, false);
                }
            }
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFillPattern(
                uint_fast8_t pattern, uint_fast32_t bgColor, uint_fast32_t fillColor,
                bool needRecreate, bool useTransparency
        ) {
            bool setStandardPattern = true;
            if(pattern == USER_FILL){
                if(!_haveUserPattern){
                    pattern = CLOSE_DOT_FILL;
                } else {
                    setStandardPattern = false;
                    pattern = USER_PATTERN_INDEX;
                    createFPattern(
                            USER_PATTERN_INDEX, bgColor, fillColor,
                            reinterpret_cast<const char *>(_userPattern),
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
            setTexture(pattern, useTransparency);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFillPattern(uint_fast8_t pattern, bool useTransparency) {
            setFillPattern(pattern, 0, 0, false, useTransparency);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFillPatternBgColor(
                uint_fast8_t pattern, uint_fast32_t bgColor, bool useTransparency
        ) {
            setFillPattern(pattern, bgColor, bgColor, true, useTransparency);
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setFillPatternBgFillColor(
                uint_fast8_t pattern, uint_fast32_t bgColor, uint_fast32_t fillColor) {
            setFillPattern(pattern, bgColor, fillColor, true, true);
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
            std::copy(pattern, pattern + FPATTERN_SIZE, _userPattern);
            _haveUserPattern = true;
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
            if(_textures[index].texture == NULL) {
                needCreate = true;
                if (_device->CreateTexture(
                        FPATTERN_W, FPATTERN_H, 1, 0,
                        _textureFormat, D3DPOOL_MANAGED, &_textures[index].texture, NULL
                ) != D3D_OK) {
                    THROW_EXC_CODE(Exception, DX9_CANT_CREATE_TEXTURE, std::wstring(L"Can't create pattern texture"));
                };
            }
            if(
                    needCreate || forceCreate ||
                    (needRecreate && _textures[index].bgColor != bgColor) ||
                    (needRecreate && useTransparency && _textures[index].fillColor != fillColor)
            ) {
                D3DLOCKED_RECT outRect;
                _textures[index].texture->LockRect(0, &outRect, NULL, 0);
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
                _textures[index].texture->UnlockRect(0);
                _textures[index].bgColor = bgColor;
                _textures[index].fillColor = fillColor;
            }
        }

        template<uint_fast32_t DispMode>
        void PatternTexturesHelper<DispMode>::setTexture(uint_fast8_t index, bool useTransparency) {
            if(index >= NUM_TOTAL_FPATTERNS){
                THROW_EXC_CODE(WException, UNREACHABLE_CODE, std::wstring(L"Bad pattern index"));
            }
            _device->SetTexture(0, _textures[index].texture);
            _device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            _device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            _device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            if(useTransparency && !_haveConstantSupport){
                _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
                _device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
            } else {
                if (_haveConstantSupport) {
                    _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CONSTANT);
                } else {
                    _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
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
        PatternTexturesHelper<DispMode>::~PatternTexturesHelper() {
            _device->SetTexture(0, NULL);
            for(uint_fast8_t i = 0; i < NUM_TOTAL_FPATTERNS; i++){
                if(_textures[i].texture != NULL){
                    _textures[i].texture->Release();
                }
            }
            delete [] _userPattern;
        }

        template class PatternTexturesHelper<ColorFormat::A8R8G8B8>;
        template class PatternTexturesHelper<ColorFormat::A8>;
        template class PatternTexturesHelper<ColorFormat::AR5G5B5>;
        template class PatternTexturesHelper<ColorFormat::A4R4G4B4>;
    }
}

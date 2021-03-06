#include <WException.h>
#include "DeviceFeatures.h"
#include "Exception.h"

namespace directgraph{
    namespace dx9{

        ColorFormat::Format DeviceFeatures::getPatternTexFormat() {
            if((_d3d->CheckDeviceFormat(
                    D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL,
                    _displayMode.Format,
                    0,
                    D3DRTYPE_TEXTURE,
                    D3DFMT_A8
            ) == D3D_OK) && supportsTexConst()){
                return ColorFormat::A8;
            }
            if(_d3d->CheckDeviceFormat(
                    D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL,
                    _displayMode.Format,
                    0,
                    D3DRTYPE_TEXTURE,
                    D3DFMT_A8R8G8B8
            ) == D3D_OK){
                return ColorFormat::A8R8G8B8;
            }
            if(_d3d->CheckDeviceFormat(
                    D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL,
                    _displayMode.Format,
                    0,
                    D3DRTYPE_TEXTURE,
                    D3DFMT_A1R5G5B5
            ) == D3D_OK){
                return ColorFormat::AR5G5B5;
            }
            THROW_EXC_CODE(
                    Exception, DX9_NO_TEXTURE_FORMAT, std::wstring(L"No suitable format for pattern textures")
            );
        }

        ColorFormat::Format DeviceFeatures::getTranspPatternTexFormat() {
            if(_d3d->CheckDeviceFormat(
                    D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL,
                    _displayMode.Format,
                    0,
                    D3DRTYPE_TEXTURE,
                    D3DFMT_A4R4G4B4
            ) == D3D_OK){
                return ColorFormat::A4R4G4B4;
            }
            THROW_EXC_CODE(
                    Exception, DX9_NO_TEXTURE_FORMAT, std::wstring(L"No suitable format for transparent pattern textures")
            );
        }

        bool DeviceFeatures::supportsTexConst() {
            return (_caps.PrimitiveMiscCaps & D3DPMISCCAPS_PERSTAGECONSTANT) == D3DPMISCCAPS_PERSTAGECONSTANT;
        }

        DeviceFeatures::DeviceFeatures(IDirect3D9 *d3d): _d3d(d3d), _caps() {
            HRESULT hRes = _d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &_caps);
            if (FAILED(hRes)) {
                _devType = D3DDEVTYPE_REF;
                _d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, _devType, &_caps);
            } else {
                _devType = D3DDEVTYPE_HAL;
            }
            if (_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &_displayMode) != D3D_OK) {
                THROW_EXC_CODE(
                        Exception, DX9_CANT_GET_DISPLAY_MODE, std::wstring(L"Can't get display mode")
                );
            }
        }

        void DeviceFeatures::getDeviceFlags(D3DDEVTYPE &devType, DWORD &flags) {
            devType = _devType;
            if(_devType == D3DDEVTYPE_HAL){
                if (_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
                    flags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
                } else {
                    flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
                }
            } else {
                flags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            }
        }

        D3DFORMAT DeviceFeatures::getDisplayMode() {
            return _displayMode.Format;
        }

        ColorFormat::Format DeviceFeatures::getImageTexFormat() {
            switch (_displayMode.Format) {
                case D3DFMT_R8G8B8:
                case D3DFMT_X8R8G8B8:
                case D3DFMT_A8R8G8B8:
                    return ColorFormat::A8R8G8B8;
                case D3DFMT_R5G6B5:
                    return ColorFormat::AR5G5B5;
                default:
                    THROW_EXC_CODE(
                            Exception,
                            DX9_UNSUPPORTED_DISPLAY_FORMAT,
                            std::wstring(L"Unsupported display format")
                    );
            }
        }

        IFeatures::ShaderVersion DeviceFeatures::getVertexShaderVer() {
            uint_fast32_t verNum = _caps.VertexShaderVersion;
            IFeatures::ShaderVersion version = {
                    static_cast<uint_fast8_t>((verNum >> 8) & 0xFF),
                    static_cast<uint_fast8_t>(verNum & 0xFF)
            };
            return version;
        }

        IFeatures::ShaderVersion DeviceFeatures::getPixelShaderVer() {
            uint_fast32_t verNum = _caps.PixelShaderVersion;
            IFeatures::ShaderVersion version = {
                    static_cast<uint_fast8_t>((verNum >> 8) & 0xFF),
                    static_cast<uint_fast8_t>(verNum & 0xFF)
            };
            return version;
        }
    }
}
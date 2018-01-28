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
                    D3DFMT_A1R5G5B5
            ) == D3D_OK){
                return ColorFormat::AR5G5B5;
            }
            THROW_EXC_CODE(
                    Exception, DX9_NO_TEXTURE_FORMAT, std::wstring(L"No suitable format for pattern textures")
            );
        }

        bool DeviceFeatures::supportsTexConst() {
            return (_caps.PrimitiveMiscCaps & D3DPMISCCAPS_PERSTAGECONSTANT) == D3DPMISCCAPS_PERSTAGECONSTANT;
        }

        DeviceFeatures::DeviceFeatures(IDirect3D9 *d3d): _d3d(d3d), _caps({}) {
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
    }
}
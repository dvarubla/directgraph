#pragma once

#include "IFeatures.h"

namespace directgraph {
    namespace dx9 {
        class DeviceFeatures: public IFeatures {
        private:
            IDirect3D9 *_d3d;
            D3DCAPS9 _caps;
            D3DDEVTYPE _devType;
            D3DDISPLAYMODE _displayMode;
        public:
            DeviceFeatures(IDirect3D9 *d3d);

            D3DFORMAT getDisplayMode();

            ColorFormat::Format getImageTexFormat();

            ColorFormat::Format getPatternTexFormat();

            void getDeviceFlags(D3DDEVTYPE &devType, DWORD &flags);

            bool supportsTexConst();
        };
    }
}


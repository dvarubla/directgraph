#pragma once

#include <dx9/IFeatures.h>
#include <gmock/gmock.h>

namespace directgraph {
    namespace dx9 {
        class FeaturesStub: public IFeatures {
        public:
            MOCK_METHOD2(getDeviceFlags, void (D3DDEVTYPE &devType, DWORD &flags));

            MOCK_METHOD0(getImageTexFormat, ColorFormat::Format());

            MOCK_METHOD0(getTranspPatternTexFormat, ColorFormat::Format());

            MOCK_METHOD0(getPatternTexFormat, ColorFormat::Format());

            MOCK_METHOD0(getDisplayMode, D3DFORMAT ());

            MOCK_METHOD0(supportsTexConst, bool ());

            MOCK_METHOD0(getVertexShaderVer, ShaderVersion ());

            MOCK_METHOD0(getPixelShaderVer, ShaderVersion ());
        };
    }
}



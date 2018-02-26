#pragma once

#include <FormatFinder.h>
#include <windows.h>
#include <d3d9.h>

namespace directgraph {
    namespace dx9 {
        class IFeatures {
        public:
            virtual void getDeviceFlags(D3DDEVTYPE &devType, DWORD &flags) = 0;
            virtual ColorFormat::Format getPatternTexFormat() = 0;
            virtual ColorFormat::Format getImageTexFormat() = 0;
            virtual D3DFORMAT getDisplayMode() = 0;
            virtual bool supportsTexConst() = 0;
            struct ShaderVersion{
                bool operator <=(const ShaderVersion &other){
                    return
                            major <= other.major &&
                            ((major == other.major && minor <= other.minor) || major != other.major)
                    ;
                }
                uint_fast8_t major;
                uint_fast8_t minor;
            };
            virtual ShaderVersion getVertexShaderVer() = 0;
            virtual ShaderVersion getPixelShaderVer() = 0;
            virtual ~IFeatures(){}
        };
    }
}


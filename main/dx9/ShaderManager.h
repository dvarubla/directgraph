#pragma once

#include "IFeatures.h"

namespace directgraph {
    namespace dx9 {
        class ShaderManager {
        private:
            IDirect3DDevice9 *_device;
            IDirect3DVertexShader9 *_centerBarV11Shader;
            IDirect3DPixelShader9 *_ellipseP14Shader;
            IDirect3DVertexDeclaration9 *_centerBarV11Decl;
            void createVertexShader(uint_fast32_t name, uint_fast32_t type, IDirect3DVertexShader9 *&shader);
            void createPixelShader(uint_fast32_t name, uint_fast32_t type, IDirect3DPixelShader9 *&shader);
            bool _supportsEllipse;
            void tryDeleteRes();
        public:
            ShaderManager(IFeatures *features, IDirect3DDevice9 *device);
            ~ShaderManager();
            bool supportsEllipse();
            void setEllipse();
            void removeShaders();
        };
    }
}



#pragma once

#include "IFeatures.h"

namespace directgraph {
    namespace dx9 {
        class ShaderManager {
        private:
            IDirect3DDevice9 *_device;
            IDirect3DVertexShader9 *_centerBarV11Shader;
            IDirect3DPixelShader9 *_ellipseP14Shader;
            IDirect3DVertexShader9 *_texturedBarV11Shader;
            IDirect3DPixelShader9 *_texturedBarP14Shader;
            IDirect3DVertexDeclaration9 *_centerBarV11Decl;
            IDirect3DVertexDeclaration9 *_texturedBarV11Decl;
            void createVertexShader(uint_fast32_t name, uint_fast32_t type, IDirect3DVertexShader9 *&shader);
            void createPixelShader(uint_fast32_t name, uint_fast32_t type, IDirect3DPixelShader9 *&shader);
            bool _supportsEllipse;
            bool _supportsTexturedBar;
            void tryDeleteRes();
        public:
            ShaderManager(IFeatures *features, IDirect3DDevice9 *device);
            ~ShaderManager();
            bool supportsEllipse();
            bool supportsTexturedBar();
            void setEllipse();
            void setTexturedBar();
            void removeShaders();
        };
    }
}



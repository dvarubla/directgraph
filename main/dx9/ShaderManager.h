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
            IDirect3DVertexShader9 *_texturedBarV11Shader;
            IDirect3DPixelShader9 *_texturedBarP14Shader;
            IDirect3DVertexDeclaration9 *_texturedBarV11Decl;
            IDirect3DVertexShader9 *_texturedCenterBarV11Shader;
            IDirect3DPixelShader9 *_texturedEllipseP14Shader;
            IDirect3DVertexDeclaration9 *_texturedCenterBarV11Decl;
            void createVertexShader(uint_fast32_t name, uint_fast32_t type, IDirect3DVertexShader9 *&shader);
            void createPixelShader(uint_fast32_t name, uint_fast32_t type, IDirect3DPixelShader9 *&shader);
            bool _supportsEllipse;
            bool _supportsTexturedBar;
            bool _supportsTexturedEllipse;
            void tryDeleteRes();
            void createEllipseShaders(
                    const IFeatures::ShaderVersion &vertexVer,
                    const IFeatures::ShaderVersion &pixelVer
            );
            void createTexturedBarShaders(
                    const IFeatures::ShaderVersion &vertexVer,
                    const IFeatures::ShaderVersion &pixelVer
            );
            void createTexturedEllipseShaders(
                    const IFeatures::ShaderVersion &vertexVer,
                    const IFeatures::ShaderVersion &pixelVer
            );
        public:
            ShaderManager(IFeatures *features, IDirect3DDevice9 *device);
            ~ShaderManager();
            bool supportsEllipse();
            bool supportsTexturedBar();
            bool supportsTexturedEllipse();
            bool supportsShaders();
            void setEllipse();
            void setTexturedBar();
            void setTexturedEllipse();
            void removeShaders();
        };
    }
}



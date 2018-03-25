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
            IDirect3DVertexShader9 *_texturedCenterRectangleV11Shader;
            IDirect3DPixelShader9 *_texturedRectangleP20Shader;
            IDirect3DVertexDeclaration9 *_texturedCenterRectangleV11Decl;
            IDirect3DVertexShader9 *_centerRectangleV11Shader;
            IDirect3DPixelShader9 *_rectangleP14Shader;
            IDirect3DVertexDeclaration9 *_centerRectangleV11Decl;
            void createVertexShader(uint_fast32_t name, uint_fast32_t type, IDirect3DVertexShader9 *&shader);
            void createPixelShader(uint_fast32_t name, uint_fast32_t type, IDirect3DPixelShader9 *&shader);
            bool _supportsEllipse;
            bool _supportsTexturedBar;
            bool _supportsTexturedEllipse;
            bool _supportsTexturedRectangle;
            bool _supportsRectangle;
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
            void createTexturedRectangleShaders(
                    const IFeatures::ShaderVersion &vertexVer,
                    const IFeatures::ShaderVersion &pixelVer
            );
            void createRectangleShaders(
                    const IFeatures::ShaderVersion &vertexVer,
                    const IFeatures::ShaderVersion &pixelVer
            );
        public:
            ShaderManager(IFeatures *features, IDirect3DDevice9 *device);
            ~ShaderManager();
            bool supportsEllipse();
            bool supportsTexturedBar();
            bool supportsTexturedEllipse();
            bool supportsTexturedRectangle();
            bool supportsRectangle();
            bool supportsShaders();
            void setEllipse();
            void setTexturedBar();
            void setTexturedEllipse();
            void setTexturedRectangle();
            void setRectangle();
            void removeShaders();
        };
    }
}



#pragma once

#include <resources/common_res.h>
#include <array>
#include "IFeatures.h"

namespace directgraph {
    namespace dx9 {
        class ShaderManager {
        private:
            enum VertexShaderId{
                CENTER_BAR_V1_1_SHADER,
                TEXTURED_BAR_V1_1_SHADER,
                TEXTURED_CENTER_BAR_V1_1_SHADER,
                TEXTURED_CENTER_RECTANGLE_V1_1_SHADER,
                CENTER_RECTANGLE_V1_1_SHADER,
                TEXTURED_LINE_V1_1_SHADER,
                ELLIPSE_OUTLINE_V1_1_SHADER,
                ELLIPSE_WITH_OUTLINE_V1_1_SHADER,
                TEXTURED_ELLIPSE_WITH_OUTLINE_V1_1_SHADER,
                TOTAL_SHADERS
            };
            const static int FIRST_VERTEX_SHADER = CENTER_BAR_V1_1;
            enum PixelShaderId{
                ELLIPSE_P1_4_SHADER,
                TEXTURED_BAR_P1_4_SHADER,
                TEXTURED_ELLIPSE_P1_4_SHADER,
                TEXTURED_RECTANGLE_P2_0_SHADER,
                RECTANGLE_P1_4_SHADER,
                TEXTURED_LINE_P1_4_SHADER,
                ELLIPSE_OUTLINE_P3_0_SHADER,
                ELLIPSE_WITH_OUTLINE_P3_0_SHADER,
                TEXTURED_ELLIPSE_WITH_OUTLINE_P3_0_SHADER
            };
            const static int FIRST_PIXEL_SHADER = ELLIPSE_P1_4;
            struct ShaderInfo{
                IDirect3DVertexShader9 *vertexShader;
                IDirect3DPixelShader9 *pixelShader;
                IDirect3DVertexDeclaration9 *vertexDecl;
                uint_fast32_t vertexShaderId;
                uint_fast32_t pixelShaderId;
                IFeatures::ShaderVersion vertexShaderVer;
                IFeatures::ShaderVersion pixelShaderVer;
                bool haveSupport;
            };
            std::array<ShaderInfo, TOTAL_SHADERS> _shaders;
            IDirect3DDevice9 *_device;
            bool _haveShaderSupport;
            void createVertexShader(uint_fast32_t name, uint_fast32_t type, IDirect3DVertexShader9 *&shader);
            void createPixelShader(uint_fast32_t name, uint_fast32_t type, IDirect3DPixelShader9 *&shader);

            void setShader(ShaderManager::PixelShaderId id);
            void tryDeleteRes();
            void createShader(
                    PixelShaderId id,
                    const IFeatures::ShaderVersion &vertexVer,
                    const IFeatures::ShaderVersion &pixelVer
            );
            bool checkSupport(uint_fast16_t id);
        public:
            ShaderManager(IFeatures *features, IDirect3DDevice9 *device);
            ~ShaderManager();
            bool supportsEllipse();
            bool supportsTexturedBar();
            bool supportsTexturedEllipse();
            bool supportsTexturedRectangle();
            bool supportsRectangle();
            bool supportsTexturedLine();
            bool supportsEllipseOutline();
            bool supportsEllipseWithOutline();
            bool supportsTexturedEllipseWithOutline();
            bool supportsShaders();
            void setEllipse();
            void setTexturedBar();
            void setTexturedEllipse();
            void setTexturedRectangle();
            void setRectangle();
            void setTexturedLine();
            void setEllipseOutline();
            void setEllipseWithOutline();
            void setTexturedEllipseWithOutline();
            void removeShaders();
        };
    }
}



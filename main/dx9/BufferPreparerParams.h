#pragma once

#include "ShaderManager.h"
#include "PatternTexturesHelper.h"

namespace directgraph {
    namespace dx9 {
        class BufferPreparerParams {
        private:
            ShaderManager *_shaderMan;
            bool _needRecreateTexture;
            UCoords _maxCrds, _pxTextureCrds;
            uint_fast32_t _maxDepthValues;
            float _depthIncrement;
        public:
            bool supportsEllipse();
            bool supportsTexturedBar();
            bool supportsTexturedEllipse();
            bool needRecreateTexture();
            UCoords getMaxCoords();
            UCoords getPxTextureCoords();
            float getDepthIncrement();
            uint_fast32_t getMaxDepthValues();

            BufferPreparerParams(
                    ShaderManager *shaderMan,
                    bool needRecreateTexture,
                    const UCoords &maxCrds,
                    const UCoords &pxTextureCrds,
                    uint_fast32_t maxDepthValues
            );
        };
    }
}


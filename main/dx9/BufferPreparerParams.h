#pragma once

#include "ShaderManager.h"
#include "PatternTexturesHelper.h"

namespace directgraph {
    namespace dx9 {
        class BufferPreparerParams {
        private:
            ShaderManager *_shaderMan;
            bool _needRecreateTexture;
            uint_fast32_t _width;
            uint_fast32_t _height;
            uint_fast32_t _pxTextureWidth;
            uint_fast32_t _pxTextureHeight;
            uint_fast32_t _maxDepthValues;
            float _depthIncrement;
        public:
            bool supportsEllipse();
            bool supportsTexturedBar();
            bool supportsTexturedEllipse();
            bool needRecreateTexture();
            uint_fast32_t getWidth();
            uint_fast32_t getHeight();
            uint_fast32_t getPxTextureWidth();
            uint_fast32_t getPxTextureHeight();
            float getDepthIncrement();
            uint_fast32_t getMaxDepthValues();

            BufferPreparerParams(
                    ShaderManager *shaderMan,
                    bool needRecreateTexture,
                    uint_fast32_t width, uint_fast32_t height,
                    uint_fast32_t pxTextureWidth, uint_fast32_t pxTextureHeight,
                    uint_fast32_t maxDepthValues
            );
        };
    }
}


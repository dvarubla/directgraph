#include "BufferPreparerParams.h"

namespace directgraph{
    namespace dx9{

        bool BufferPreparerParams::supportsEllipse() {
            return _shaderMan->supportsEllipse();
        }

        bool BufferPreparerParams::supportsTexturedBar() {
            return _shaderMan->supportsTexturedBar();
        }

        bool BufferPreparerParams::supportsTexturedEllipse() {
            return _shaderMan->supportsTexturedEllipse();
        }

        bool BufferPreparerParams::needRecreateTexture() {
            return _needRecreateTexture;
        }

        uint_fast32_t BufferPreparerParams::getWidth() {
            return _width;
        }

        uint_fast32_t BufferPreparerParams::getHeight() {
            return _height;
        }

        uint_fast32_t BufferPreparerParams::getPxTextureWidth() {
            return _pxTextureWidth;
        }

        uint_fast32_t BufferPreparerParams::getPxTextureHeight() {
            return _pxTextureHeight;
        }

        BufferPreparerParams::BufferPreparerParams(
                ShaderManager *shaderMan, bool needRecreateTexture,
                uint_fast32_t width, uint_fast32_t height,
                uint_fast32_t pxTextureWidth, uint_fast32_t pxTextureHeight,
                uint_fast32_t maxDepthValues
        ) : _shaderMan(shaderMan), _needRecreateTexture(needRecreateTexture),
            _width(width), _height(height), _pxTextureWidth(pxTextureWidth), _pxTextureHeight(pxTextureHeight),
            _maxDepthValues(maxDepthValues), _depthIncrement(1.0f / (maxDepthValues - 1)) {
        }

        float BufferPreparerParams::getDepthIncrement() {
            return _depthIncrement;
        }

        uint_fast32_t BufferPreparerParams::getMaxDepthValues() {
            return _maxDepthValues;
        }
    }
}

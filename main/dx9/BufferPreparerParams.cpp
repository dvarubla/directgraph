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

        bool BufferPreparerParams::supportsTexturedRectangle() {
            return _shaderMan->supportsTexturedRectangle();
        }

        bool BufferPreparerParams::supportsRectangle() {
            return _shaderMan->supportsRectangle();
        }

        bool BufferPreparerParams::supportsTexturedLine() {
            return _shaderMan->supportsTexturedLine();
        }

        bool BufferPreparerParams::needRecreateTexture() {
            return _needRecreateTexture;
        }

        BufferPreparerParams::BufferPreparerParams(
                ShaderManager *shaderMan,
                bool needRecreateTexture,
                const UCoords &maxCrds,
                const UCoords &pxTextureCrds,
                uint_fast32_t maxDepthValues
        ) : _shaderMan(shaderMan), _needRecreateTexture(needRecreateTexture),
            _maxCrds(maxCrds), _pxTextureCrds(pxTextureCrds),
            _maxDepthValues(maxDepthValues), _depthIncrement(1.0f / (maxDepthValues)) {
        }

        float BufferPreparerParams::getDepthIncrement() {
            return _depthIncrement;
        }

        uint_fast32_t BufferPreparerParams::getMaxDepthValues() {
            return _maxDepthValues;
        }

        UCoords BufferPreparerParams::getMaxCoords() {
            return _maxCrds;
        }

        UCoords BufferPreparerParams::getPxTextureCoords() {
            return _pxTextureCrds;
        }
    }
}

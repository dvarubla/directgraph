#include <VertexCreator.h>
#include "PixelContainerDrawer.h"

namespace directgraph{
    namespace dx9{
        void PixelContainerDrawer::getItemState(ItemState &state) {
            _propMan->setPropP(state, PropertyName::PIXEL_CONTAINER, _curItem.data.pixelContainer);
        }

        NumVertices PixelContainerDrawer::getNumVertices(bool isFirst) {
            NumVertices res = {(isFirst) ? 0u : 2u, VERTICES_IN_QUAD};
            return res;
        }

        TypeSize PixelContainerDrawer::getTypeSize() {
            TypeSize res = {sizeof(TexturedVertex), DrawDataType::TEXTURED_VERTEX};
            return res;
        }

        void PixelContainerDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &, float curZ) {
            Rectangle coords = _curItem.data.pixelContainer->getCoords();
            curVertMem = _simplePrimHelper->genTexQuad(
                    curVertMem,
                    genCoords(coords.left, coords.top), genCoords(coords.right, coords.bottom),
                    _bufPrepParams->getPxTextureCoords(),
                    curZ
            );
        }

        StartEndCoords PixelContainerDrawer::getStartEndCoords() {
            Rectangle coords = _curItem.data.pixelContainer->getCoords();
            StartEndCoords res = {genCoords(coords.left, coords.top), genCoords(coords.right, coords.bottom)};
            return res;
        }

        void PixelContainerDrawer::genDegenerates(void *&, const Coords &, const Coords &, float ) {
        }

        PixelContainerDrawer::~PixelContainerDrawer() {
        }

        void PixelContainerDrawer::setItem(const QueueItem &item) {
           _curItem = item;
        }

        PixelContainerDrawer::PixelContainerDrawer(
                BufferPreparerParams *bufPrepParams, PropertyManager *propMan,
                SimplePrimHelper *simplePrimHelper
        ) :  _bufPrepParams(bufPrepParams),
            _propMan(propMan), _simplePrimHelper(simplePrimHelper){

        }

        bool PixelContainerDrawer::isSemiTransparent() {
            return true;
        }
    }
}
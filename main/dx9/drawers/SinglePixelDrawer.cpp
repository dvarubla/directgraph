#include <VertexCreator.h>
#include "SinglePixelDrawer.h"

namespace directgraph{
    namespace dx9{
        void SinglePixelDrawer::getItemState(ItemState &) {
        }

        NumVertices SinglePixelDrawer::getNumVertices(bool isFirst) {
            NumVertices res = {(isFirst) ? 0u : 2u, VERTICES_IN_QUAD};
            return res;
        }

        TypeSize SinglePixelDrawer::getTypeSize() {
            TypeSize res = {sizeof(ColorVertex), DrawDataType::COLOR_VERTEX};
            return res;
        }

        void SinglePixelDrawer::processDrawItem(void *&curVertMem, uint_fast32_t &, float curZ) {
            curVertMem = _simplePrimHelper->genQuad(curVertMem, _coords[0], _coords[1],
                                              curZ, _curItem.data.singlePixel.color
            );
        }

        StartEndCoords SinglePixelDrawer::getStartEndCoords() {
            StartEndCoords res = {
                    _coords[0], _coords[1]
            };
            return res;
        }

        void SinglePixelDrawer::genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds,
                                               float curZ) {
            curVertMem = _degenerateHelper->genDegenerate(
                    curVertMem, startCrds, endCrds,
                    curZ
            );
        }

        SinglePixelDrawer::~SinglePixelDrawer() {

        }

        void SinglePixelDrawer::setItem(const QueueItem &item) {
            _curItem = item;
            genPixelCoords();
        }

        SinglePixelDrawer::SinglePixelDrawer(BufferPreparerParams *bufPrepParams,
                                             PropertyManager *propMan, SimplePrimHelper *simplePrimHelper,
                                             DegenerateHelper *degenerateHelper
        ): _bufPrepParams(bufPrepParams),
           _propMan(propMan), _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper){
        }

        void SinglePixelDrawer::genPixelCoords() {
            _coords[0] = subtHalfPixel(genFCoords(_curItem.data.singlePixel.x, _curItem.data.singlePixel.y));
            _coords[1] = subtHalfPixel(genFCoords(_curItem.data.singlePixel.x + 1, _curItem.data.singlePixel.y + 1));
        }
    }
}
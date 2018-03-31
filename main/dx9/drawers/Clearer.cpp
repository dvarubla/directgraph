#include <VertexCreator.h>
#include "Clearer.h"

namespace directgraph{
    namespace dx9{
        void Clearer::getItemState(ItemState &) {
        }

        NumVertices Clearer::getNumVertices(bool isFirst) {
            NumVertices res = {(isFirst) ? 0u : 2u, VERTICES_IN_QUAD};
            return res;
        }

        TypeSize Clearer::getTypeSize() {
            TypeSize res = {sizeof(ColorVertex), DrawDataType::COLOR_VERTEX};
            return res;
        }

        void Clearer::processDrawItem(void *&curVertMem, uint_fast32_t &,
                                      float curZ) {

            curVertMem = _simplePrimHelper->genQuad(curVertMem,
                                              _coords[0], _coords[1], curZ,
                                              0xFFFFFF
            );
        }

        StartEndCoords Clearer::getStartEndCoords() {
            StartEndCoords res = {_coords[0], _coords[1]};
            return res;
        }

        void Clearer::genDegenerates(void *&curVertMem, const FCoords &startCrds, const FCoords &endCrds,
                                     float curZ) {
            curVertMem = _degenerateHelper->genDegenerate(
                    curVertMem, startCrds, endCrds, curZ
            );
        }

        Clearer::~Clearer() {

        }

        Clearer::Clearer(
                BufferPreparerParams *bufPrepParams,
                PropertyManager *propMan, SimplePrimHelper *simplePrimHelper,
                DegenerateHelper *degenerateHelper
        ) : _bufPrepParams(bufPrepParams), _propMan(propMan),
            _simplePrimHelper(simplePrimHelper), _degenerateHelper(degenerateHelper){
        }

        void Clearer::genCoords() {
            _coords[0] = subtHalfPixel(genFCoords(0, 0)),
            _coords[1] = subtHalfPixel(genFCoords(_bufPrepParams->getMaxCoords().x, _bufPrepParams->getMaxCoords().y));
        }

        void Clearer::setItemState(const ItemState &) {
            genCoords();
        }
    }
}
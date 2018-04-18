#include <algorithm>
#include <main/patterns.h>
#include <misc.h>
#include <strsafe.h>
#include "EllipseHelper.h"
#include "util.h"

#undef min
#undef max

namespace directgraph{
    namespace dx9{
        Ellipse EllipseHelper::genEllipse(const Coords &centerCrds, const UCoords &radiusCrds,
                                          uint_fast16_t startAngle, uint_fast16_t endAngle,
                                          bool textured) {
            _coordVect.clear();
            _textured = textured;
            if(textured) {
                _texCoordVect.clear();
                _minCoords = genDCoords(
                        centerCrds.x - static_cast<uint_fast32_t>(radiusCrds.x),
                        centerCrds.y - static_cast<uint_fast32_t>(radiusCrds.y)
                );
            }
            QuadrantStatus qs[NUM_QUADRANTS];
            _swap = radiusCrds.y > radiusCrds.x;
            UCoords actRadiusCrds = radiusCrds;
            if(_swap){
                std::swap(actRadiusCrds.x, actRadiusCrds.y);
            }
            genQuadrantData(startAngle, endAngle, qs, false);
            ICoordVect ellipsePixels = getEllipsePixels(actRadiusCrds);
            std::vector<FullRect> ellipseRects = genEllipseRects(ellipsePixels);
            std::vector<PartRect> partRects;
            drawRects(partRects, ellipseRects, centerCrds, qs);

            Ellipse res;
            res.coords = _coordVect;
            if(textured){
                res.texCoords = _texCoordVect;
            }
            return res;
        }

        ICoordVect EllipseHelper::getEllipsePixels(const UCoords &radiusCoords) {
            ICoordVect res;
            uint_fast32_t a = radiusCoords.x;
            uint_fast32_t b = radiusCoords.y;
            uint_fast32_t x = a, y = 0;
            while (true) {
                res.push_back(genCoords(x, y));
                if ((b * b * (x - 0.5) * (x - 0.5) + 1.0 * a * a * (y + 1) * (y + 1) - 1.0 * a * a * b * b) > 0) {
                    x--;
                }
                y++;
                if (b * b * x < a * a * y) {
                    break;
                }
            }
            if(x != 0) {
                while (true) {
                    res.push_back(genCoords(x, y));
                    if ((b * b * (x - 1.0) * (x - 1.0) + a * a * (y + 0.5) * (y + 0.5) - 1.0 * a * a * b * b) < 0) {
                        y++;
                    }
                    x--;
                    if (x == 0) {
                        res.push_back(genCoords(x, y));
                        break;
                    }
                }
            } else {
                res.push_back(genCoords(x, y));
            }
            return res;
        }

        std::vector<EllipseHelper::FullRect> EllipseHelper::genEllipseRects(const ICoordVect &pixels) {
            std::vector<FullRect> res;
            uint_fast32_t i = 1;
            Coords prev = pixels[0];
            uint_fast32_t size = pixels.size();
            while (true) {
                if (pixels[i].x != prev.x || pixels[i + 1].y == pixels[i].y) {
                    while (i != size && pixels[i].y == pixels[i - 1].y) {
                        i++;
                    }
                    if(pixels[i - 1].x != 0) {
                        FullRect tRect = {pixels[i - 1].x - 1,  pixels[i - 1].y, prev.y};
                        res.push_back(tRect);
                    }
                    if (i >= size - 1) {
                        break;
                    }
                    prev = pixels[i];
                }
                i++;
            }
            return res;
        }

        void
        EllipseHelper::genRectPoints(int_fast32_t x1, int_fast32_t y1, int_fast32_t x2,
                                     int_fast32_t y2) {
            if(_coordVect.size() != 0){
                _coordVect.push_back(_coordVect.back());
                _coordVect.push_back(subtHalfPixel(genFCoords(x1, y1)));
            }
            _coordVect.push_back(subtHalfPixel(genFCoords(x1, y1)));
            _coordVect.push_back(subtHalfPixel(genFCoords(x1, y2)));
            _coordVect.push_back(subtHalfPixel(genFCoords(x2, y1)));
            _coordVect.push_back(subtHalfPixel(genFCoords(x2, y2)));

            if(_textured){
                FCoords crds1 = _texCrdCalc->calcBarCoords(_minCoords, genDCoords(x1, y1)).end;
                if(_texCoordVect.size() != 0){
                    _texCoordVect.push_back(_texCoordVect.back());
                    _texCoordVect.push_back(crds1);
                }
                _texCoordVect.push_back(crds1);
                _texCoordVect.push_back(_texCrdCalc->calcBarCoords(_minCoords, genDCoords(x1, y2)).end);
                _texCoordVect.push_back(_texCrdCalc->calcBarCoords(_minCoords, genDCoords(x2, y1)).end);
                _texCoordVect.push_back(_texCrdCalc->calcBarCoords(_minCoords, genDCoords(x2, y2)).end);
            }
        }

        void EllipseHelper::drawRects(
                const std::vector<PartRect> &partRects,
                const std::vector<FullRect> &fullRects, const Coords &centerCrds,
                const EllipseHelper::QuadrantStatus (&qs)[NUM_QUADRANTS]) {
            uint_fast32_t partRSize = partRects.size();
            uint_fast32_t fullRSize = fullRects.size();
            for(uint_fast32_t i = 0; i < fullRSize; i++) {
                for (uint_fast8_t m = 0; m < NUM_QUADRANTS; m += 2) {
                    if (qs[m] == INSIDE && qs[m + 1] == INSIDE) {
                        int_fast32_t curY2 = fullRects[i].y2;
                        if (partRSize == 0 && m == 0 && i == 0) {
                            curY2++;
                        }
                        drawFullRect(m, fullRects[i].x, fullRects[i].y1, curY2, centerCrds);
                    }
                }
            }
        }

        void EllipseHelper::drawFullRect(uint_fast8_t quadrNum,
                                         int_fast32_t x1, int_fast32_t y1, int_fast32_t y2,
                                         const Coords &centerCrds
        ) {
            if(quadrNum == 0){
                if(_swap) {
                    genRectPoints(centerCrds.x - y1, centerCrds.y - x1, centerCrds.x - y2 + 1,
                                  centerCrds.y + x1 + 1);
                } else {
                    genRectPoints(centerCrds.x - x1, centerCrds.y - y1, centerCrds.x + x1 + 1,
                                  centerCrds.y - y2 + 1);
                }
            } else {
                if(_swap) {
                    genRectPoints(centerCrds.x + y2, centerCrds.y - x1, centerCrds.x + y1 + 1, centerCrds.y + x1 + 1);
                } else {
                    genRectPoints(centerCrds.x - x1, centerCrds.y + y2, centerCrds.x + x1 + 1, centerCrds.y + y1 + 1);
                }
            }
        }

        void EllipseHelper::genQuadrantData(
                uint_fast16_t startAngle, uint_fast16_t endAngle, EllipseHelper::QuadrantStatus (&qs)[NUM_QUADRANTS],
                bool swapAngles
        ) {
            uint_fast16_t realSAngle = std::min(startAngle, endAngle);
            uint_fast16_t realEAngle = std::max(startAngle, endAngle);
            uint_fast8_t sQ = static_cast<uint_fast8_t>(realSAngle / 90);
            uint_fast8_t eQ = static_cast<uint_fast8_t>(realEAngle / 90);
            if(realEAngle % 90 == 0){
                eQ--;
            }
            for(uint_fast8_t i = 0; i < NUM_QUADRANTS; i++){
                if(i >= sQ && i <= eQ){
                    qs[i] = INSIDE;
                    if(i == sQ && realSAngle % 90 != 0){
                        qs[i] = START;
                    }
                    if(i == eQ && realEAngle % 90 != 0){
                        if(qs[i] == START){
                            qs[i] = BOTH;
                        } else {
                            qs[i] = END;
                        }
                    }
                } else {
                    qs[i] = NOTHING;
                }
                if(swapAngles){
                    if(qs[i] == NOTHING){
                        qs[i] = INSIDE;
                    } else if(qs[i] == INSIDE){
                        qs[i] = NOTHING;
                    } else if(qs[i] == START){
                        qs[i] = END;
                    } else if(qs[i] == END){
                        qs[i] = START;
                    }
                }
            }
        }

        EllipseHelper::EllipseHelper(TextureCoordsCalc *texCrdCalc) : _texCrdCalc(texCrdCalc){
        }
    }
}

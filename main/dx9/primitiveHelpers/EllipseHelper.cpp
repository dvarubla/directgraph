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

        EllipseOutline
        EllipseHelper::genOutline(const Coords &centerCrds, const UCoords &radiusCrds, uint_fast16_t startAngle,
                                  uint_fast16_t endAngle, uint_fast32_t thickness) {
            _coordVect.clear();
            _textured = false;
            QuadrantStatus qs[NUM_QUADRANTS];
            _swap = radiusCrds.y > radiusCrds.x;
            UCoords actRadiusCrds = radiusCrds;
            if(_swap){
                std::swap(actRadiusCrds.x, actRadiusCrds.y);
            }
            genQuadrantData(startAngle, endAngle, qs, false);
            thickness--;
            uint_fast32_t curTh = thickness / 2;
            ICoordVect insidePixels, outsidePixels;
            uint_fast32_t maxY;
            if(curTh == 0){
                insidePixels = getEllipsePixels(actRadiusCrds);
                maxY = actRadiusCrds.y;
            } else {
                insidePixels = getInsidePixels(actRadiusCrds, curTh, maxY);
            }

            curTh += thickness % 2;
            if(curTh == 0){
                outsidePixels = insidePixels;
            } else {
                outsidePixels = getOutsidePixels(actRadiusCrds, curTh);
            }

            Rects rects = genOutlineRects(insidePixels, outsidePixels, maxY);
            drawRects(rects.partRects, rects.fullRects, centerCrds, qs);

            EllipseOutline res;
            res.coords = _coordVect;
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

        ICoordVect EllipseHelper::getOutsidePixels(const UCoords &radiusCoords, uint_fast32_t h) {
            ICoordVect res;
            uint_fast32_t a = radiusCoords.x;
            uint_fast32_t b = radiusCoords.y;
            int_fast32_t x = a + h;
            int_fast32_t y = 0;

            double lastX = 1.0 * a * a / std::sqrt(1.0 * a * a + b * b) + h / std::sqrt(2);
            while (true) {
                res.push_back(genCoords(x, y));
                if (calcEquidistant(x - 0.5, y + 1, a, b, h) > 0) {
                    x--;
                }
                y++;
                if (x < lastX) {
                    break;
                }
            }
            if(x != 0) {
                while (true) {
                    res.push_back(genCoords(x, y));
                    if (calcEquidistant(x - 1, y + 0.5, a, b, h) < 0) {
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

        ICoordVect EllipseHelper::getInsidePixels(const UCoords &radiusCoords, uint_fast32_t h, uint_fast32_t &maxY) {
            ICoordVect res;
            uint_fast32_t a = radiusCoords.x;
            uint_fast32_t b = radiusCoords.y;
            if(h < a && h < b){
                int_fast32_t x, y;
                double lastX = 1.0 * a * a / std::sqrt(1.0 * a * a + b * b) - 1.0 * h / std::sqrt(2);
                double root = (std::sqrt(a*a - b*b) * std::sqrt(b*b - h*h))/b;
                if(h > b*b / a){
                    x = static_cast<int_fast32_t>(std::ceil(root));
                } else {
                    x = a - h;
                }
                y = 0;
                if(x > lastX) {
                    while (true) {
                        res.push_back(genCoords(x, y));
                        if (calcEquidistant(x - 0.5, y + 1, a, b, h) < 0) {
                            x--;
                        }
                        y++;
                        if (x < lastX) {
                            break;
                        }
                    }
                }
                if(x != 0) {
                    while (true) {
                        res.push_back(genCoords(x, y));
                        if (calcEquidistant(x - 1, y + 0.5, a, b, h) > 0) {
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
                maxY = b - h;
            } else {
                maxY = 0;
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

        EllipseHelper::Rects
        EllipseHelper::genOutlineRects(const ICoordVect &insidePixels, const ICoordVect &outsidePixels,
                                       uint_fast32_t maxY) {
            Rects res;
            uint_fast32_t k = 1;
            Coords prevOutside = outsidePixels[0];
            uint_fast32_t outsideSize = outsidePixels.size();
            if(maxY != 0 && insidePixels[0].y != static_cast<int_fast32_t>(maxY)) {
                uint_fast32_t j = 1;
                Coords prevInside = insidePixels[0];
                while (true) {
                    if (outsidePixels[k].x != prevOutside.x || insidePixels[j].x != prevInside.x ||
                        insidePixels[j].y == static_cast<int_fast32_t>(maxY) ||
                        insidePixels[j + 1].y == insidePixels[j].y
                            ) {
                        while (outsidePixels[k].y == outsidePixels[k - 1].y) {
                            k++;
                        }
                        if (insidePixels[j].y == prevInside.y) {
                            while (insidePixels[j].y != static_cast<int_fast32_t>(maxY) && insidePixels[j].y == prevInside.y) {
                                j++;
                            }
                        }
                        PartRect rect = {insidePixels[j - 1].x, insidePixels[j - 1].y, prevOutside.x, prevOutside.y};
                        res.partRects.push_back(rect);
                        prevOutside = outsidePixels[k];
                        prevInside = insidePixels[j];
                        if (insidePixels[j].y == static_cast<int_fast32_t>(maxY)) {
                            break;
                        }
                    }
                    k++;
                    j++;
                }
                k++;
            }
            if(k != outsideSize) {
                while (true) {
                    if (outsidePixels[k].x != prevOutside.x) {
                        while (k != outsideSize && outsidePixels[k].y == outsidePixels[k - 1].y) {
                            k++;
                        }
                        FullRect rect = {prevOutside.x, outsidePixels[k - 1].y, prevOutside.y};
                        res.fullRects.push_back(rect);
                        if (k >= outsideSize - 1) {
                            break;
                        }
                        prevOutside = outsidePixels[k];
                    }
                    k++;
                }
            } else {
                FullRect rect = {prevOutside.x, outsidePixels[k - 1].y, prevOutside.y};
                res.fullRects.push_back(rect);
            }
            return res;
        }

        void
        EllipseHelper::genRectPoints(int_fast32_t x1, int_fast32_t y1, int_fast32_t x2,
                                     int_fast32_t y2) {
            if(!_coordVect.empty()){
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

            for(uint_fast32_t i = 0; i < partRSize; i++) {
                for (uint_fast8_t m = 0; m < NUM_QUADRANTS; m++) {
                    int_fast32_t curY2 = partRects[i].y2;
                    if ((m == 0 || m == 2) && i == 0) {
                        curY2++;
                    }
                    if (qs[m] == INSIDE) {
                        drawPartRect(m, partRects[i].x1, partRects[i].y1, partRects[i].x2, curY2, centerCrds);
                    }
                }
            }

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

        void EllipseHelper::drawPartRect(uint_fast8_t quadrNum, int_fast32_t x1, int_fast32_t y1, int_fast32_t x2,
                                         int_fast32_t y2, const Coords &centerCrds) {
            switch (quadrNum){
                case 0:
                    if(_swap){
                        genRectPoints(centerCrds.x + y2, centerCrds.y - x2, centerCrds.x + y1 + 1,
                                      centerCrds.y - x1 + 1);
                    } else {
                        genRectPoints(centerCrds.x + x1, centerCrds.y - y1, centerCrds.x + x2 + 1,
                                      centerCrds.y - y2 + 1);
                    }
                    break;
                case 1:
                    if(_swap){
                        genRectPoints(centerCrds.x - y1, centerCrds.y - x2, centerCrds.x - y2 + 1,
                                      centerCrds.y - x1 + 1);
                    } else {
                        genRectPoints(centerCrds.x - x2, centerCrds.y - y1, centerCrds.x - x1 + 1,
                                      centerCrds.y - y2 + 1);
                    }
                    break;
                case 2:
                    if(_swap){
                        genRectPoints(centerCrds.x - y1, centerCrds.y + x1, centerCrds.x - y2 + 1,
                                      centerCrds.y + x2 + 1);
                    } else {
                        genRectPoints(centerCrds.x - x2, centerCrds.y + y2, centerCrds.x - x1 + 1,
                                      centerCrds.y + y1 + 1);
                    }
                    break;
                case 3:
                    if(_swap){
                        genRectPoints(centerCrds.x + y2, centerCrds.y + x1, centerCrds.x + y1 + 1,
                                      centerCrds.y + x2 + 1);
                    } else {
                        genRectPoints(centerCrds.x + x1, centerCrds.y + y2, centerCrds.x + x2 + 1,
                                      centerCrds.y + y1 + 1);
                    }
                    break;
                default:
                    break;
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

        double
        EllipseHelper::calcEquidistant(double x, double y, uint_fast32_t a, uint_fast32_t b, uint_fast32_t h) {
            double a1 = 1.0 * a * a + b * b - x * x - y * y + h * h;
            double a2 = 1.0 * a * a * b * b - 1.0 * b * b * x * x - 1.0 * a * a * y * y + 1.0 * h * h * (a * a + b * b);
            double a3 = 1.0 * h * h * a * a * b * b;

            double a312 = a3 / a1 / a2;
            return 1 - 4 * a2 / a1 / a1 - 4 * a1 * a3 / a2 / a2 + 18 * a312 - 27 * a312 * a312;
        }
    }
}

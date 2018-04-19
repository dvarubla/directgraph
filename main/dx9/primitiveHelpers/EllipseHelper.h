#pragma once

#include <stdint.h>
#include <main/util.h>
#include <TextureCoordsCalc.h>

namespace directgraph {
    namespace dx9 {
        class EllipseHelper {
        private:
            struct FullRect{
                int_fast32_t x;
                int_fast32_t y1;
                int_fast32_t y2;
            };
            struct PartRect{
                int_fast32_t x1;
                int_fast32_t y1;
                int_fast32_t x2;
                int_fast32_t y2;
            };
            struct Rects{
                std::vector<PartRect> partRects;
                std::vector<FullRect> fullRects;
            };
            const static int NUM_QUADRANTS = 4;
            ICoordVect getEllipsePixels(const UCoords &radiusCoords);
            ICoordVect getOutsidePixels(const UCoords &radiusCoords, uint_fast32_t h);
            ICoordVect getInsidePixels(const UCoords &radiusCoords, uint_fast32_t h, uint_fast32_t &maxY);
            std::vector<FullRect> genEllipseRects(const ICoordVect &pixels);
            void genOutlinePixels(uint_fast32_t thickness, const UCoords &radiusCrds,
                                  ICoordVect &insidePixels, ICoordVect &outsidePixels, uint_fast32_t &maxY);
            Rects genOutlineRects(const ICoordVect &insidePixels, const ICoordVect &outsidePixels, uint_fast32_t maxY);
            void genRectPoints(int_fast32_t x1, int_fast32_t y1, int_fast32_t x2, int_fast32_t y2);
            void drawPartRect(uint_fast8_t quadrNum,
                              int_fast32_t x1, int_fast32_t y1, int_fast32_t x2, int_fast32_t y2,
                              const Coords &centerCrds);
            void drawFullRect(uint_fast8_t quadrNum,
                              int_fast32_t x1, int_fast32_t y1, int_fast32_t y2,
                              const Coords &centerCrds);
            void drawRects(
                    const std::vector<PartRect> &partRects,
                    const std::vector<FullRect> &fullRects,
                    const Coords & centerCrds
            );

            double calcEquidistant(double x, double y, uint_fast32_t a, uint_fast32_t b, uint_fast32_t h);

            TextureCoordsCalc *_texCrdCalc;

            CoordVect _coordVect;
            CoordVect _texCoordVect;
            DCoords _minCoords;
            bool _textured;
            bool _swap;
        public:
            EllipseHelper(TextureCoordsCalc *texCrdCalc);
            Ellipse genEllipse(
                    const Coords & centerCrds,
                    const UCoords & radiusCrds,
                    bool textured
            );
            EllipseOutline genOutline(
                    const Coords & centerCrds,
                    const UCoords & radiusCrds,
                    uint_fast32_t thickness
            );

            FullEllipse genFullEllipse(
                    const Coords & centerCrds,
                    const UCoords & radiusCrds,
                    uint_fast32_t thickness,
                    bool texturedEllipse
            );
        };
    }
}



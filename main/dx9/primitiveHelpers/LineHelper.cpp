#include <util.h>
#include "LineHelper.h"

namespace directgraph {
    namespace dx9 {
        LineHelper::LineData LineHelper::calcPoints(double x1d, double y1d, double x2d, double y2d,
                                    uint_fast32_t thickness) {
            LineData res;
            res.halfT = thickness / 2.0;
            double dx = x2d - x1d, dy = y2d - y1d;
            double len = std::sqrt(dx * dx + dy * dy);
            dx /= len;
            dy /= len;
            res.dir.x = static_cast<float>(dx);
            res.dir.y = static_cast<float>(dy);
            dx *= -1;
            std::swap(dx, dy);

            res.points[0].x = static_cast<float>(x1d + res.halfT * dx);
            res.points[0].y = static_cast<float>(y1d + res.halfT * dy);

            res.points[1].x = static_cast<float>(x2d + res.halfT * dx);
            res.points[1].y = static_cast<float>(y2d + res.halfT * dy);

            res.points[2].x = static_cast<float>(x1d - res.halfT * dx);
            res.points[2].y = static_cast<float>(y1d - res.halfT * dy);

            res.points[3].x = static_cast<float>(x2d - res.halfT * dx);
            res.points[3].y = static_cast<float>(y2d - res.halfT * dy);
            res.len = std::max(
                    std::abs(res.points[1].x - res.points[0].x),
                    std::abs(res.points[1].y - res.points[0].y)
            );
            res.normal.x = static_cast<float>(dx);
            res.normal.y = static_cast<float>(dy);
            return res;
        }

        LineHelper::PointsLen
        LineHelper::getPointsLen(double x1d, double y1d, double x2d, double y2d, uint_fast32_t thickness) {
            LineData lData = calcPoints(x1d, y1d, x2d, y2d, thickness);
            FCoords offset = genFCoords(calcOffset(lData));
            lData.points[0] = addCorrOffset(lData.points[0] - offset);
            lData.points[2] = addCorrOffset(lData.points[2] - offset);
            lData.points[1] = addCorrOffset(lData.points[1] + offset);
            lData.points[3] = addCorrOffset(lData.points[3] + offset);
            lData.len = std::max(
                    std::abs(lData.points[1].x - lData.points[0].x),
                    std::abs(lData.points[1].y - lData.points[0].y)
            );
            PointsLen res;
            res.points = lData.points;
            res.len = lData.len;
            return res;
        }

        DCoords LineHelper::calcOffset(const LineHelper::LineData &lineData) {
            return lineData.dir * (lineData.halfT);
        }
    }
}

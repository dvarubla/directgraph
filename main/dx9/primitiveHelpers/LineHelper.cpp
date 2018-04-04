#include "LineHelper.h"

namespace directgraph {
    namespace dx9 {
        void LineHelper::calcPoints(int_fast32_t x1, int_fast32_t y1, int_fast32_t x2, int_fast32_t y2,
                                    uint_fast32_t thickness) {
            double
                    x1d = x1, x2d = x2,
                    y1d = y1, y2d = y2
            ;
            _halfT = thickness / 2.0;
            _extraOffset = (thickness% 2 == 0) ? -CORR_OFFSET : 0;
            double dx = x2d - x1d, dy = y2d - y1d;
            double len = std::sqrt(dx * dx + dy * dy);
            dx /= len;
            dy /= len;
            _dir.x = static_cast<float>(dx);
            _dir.y = static_cast<float>(dy);
            dx *= -1;
            std::swap(dx, dy);

            _points[0].x = static_cast<float>(x1d + _halfT * dx + _extraOffset);
            _points[0].y = static_cast<float>(y1d + _halfT * dy + _extraOffset);

            _points[1].x = static_cast<float>(x2d + _halfT * dx + _extraOffset);
            _points[1].y = static_cast<float>(y2d + _halfT * dy + _extraOffset);

            _points[2].x = static_cast<float>(x1d - _halfT * dx + _extraOffset);
            _points[2].y = static_cast<float>(y1d - _halfT * dy + _extraOffset);

            _points[3].x = static_cast<float>(x2d - _halfT * dx + _extraOffset);
            _points[3].y = static_cast<float>(y2d - _halfT * dy + _extraOffset);
            _len = std::max(
                    std::abs(x2 - x1),
                    std::abs(y2 - y1)
            ) + static_cast<int_fast32_t>(thickness) - _extraOffset * 2;
            _normal.x = static_cast<float>(dx);
            _normal.y = static_cast<float>(dy);
        }

        QuadPointsArr &LineHelper::getPoints() {
            return _points;
        }

        double LineHelper::getLen() {
            return _len;
        }

        void LineHelper::addOffsetToEnds() {
            FCoords offset = _dir * (_halfT - _extraOffset);
            _points[0] -= offset;
            _points[2] -= offset;
            _points[1] += offset;
            _points[3] += offset;
        }
    }
}

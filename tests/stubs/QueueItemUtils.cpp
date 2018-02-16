#include "QueueItemUtils.h"

namespace directgraph {
    bool operator==(Rectangle l, Rectangle r){
        return l.top == r.top && l.bottom == r.bottom && l.left == r.left && l.right == r.right;
    }

    bool operator==(QueueItem l, QueueItem r) {
        if (l.type != r.type) {
            return false;
        }
        if(l.type == QueueItem::CLEAR){
            return true;
        }
        if (l.type == QueueItem::BAR) {
            return
                    l.data.bar.left == r.data.bar.left &&
                    l.data.bar.right == r.data.bar.right &&
                    l.data.bar.top == r.data.bar.top &&
                    l.data.bar.bottom == r.data.bar.bottom;
        } else if (l.type == QueueItem::SETFILLSTYLE) {
            return
                    l.data.setfillstyle.fillStyle == r.data.setfillstyle.fillStyle &&
                    l.data.setfillstyle.color == r.data.setfillstyle.color;
        } else if(l.type == QueueItem::PIXEL_CONTAINER){
            IPixelContainer *lp = l.data.pixelContainer;
            IPixelContainer *rp = r.data.pixelContainer;
            Rectangle
                    lpC = lp->getCoords(), rpC = rp->getCoords()
            ;
            delete lp;
            delete rp;
            return lpC == rpC;
        }
        return false;
    }

    bool Comparator::operator()(QueueItem l, QueueItem r) {
        if(l.type != r.type) {
            return l.type < r.type;
        } else if(l.type == QueueItem::BAR){
            if(l.data.bar.left != r.data.bar.left){
                return l.data.bar.left < r.data.bar.left;
            } else {
                return l.data.bar.top < r.data.bar.top;
            }
        } else {
            return l.data.setfillstyle.color < r.data.setfillstyle.color;
        }
    }
}

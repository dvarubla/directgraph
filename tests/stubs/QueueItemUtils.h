#ifndef PROJECT_QUEUEITEMUTILS_H
#define PROJECT_QUEUEITEMUTILS_H

#include <QueueItem.h>

namespace directgraph {
    bool operator==(Rectangle l, Rectangle r);
    bool operator==(QueueItem l, QueueItem r);
    
    struct Comparator {
        bool operator() (QueueItem l, QueueItem r);
    };
}

#endif //PROJECT_QUEUEITEMUTILS_H

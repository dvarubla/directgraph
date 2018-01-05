#pragma once

#include <QueueItem.h>

namespace directgraph {
    bool operator==(Rectangle l, Rectangle r);
    bool operator==(QueueItem l, QueueItem r);
    
    struct Comparator {
        bool operator() (QueueItem l, QueueItem r);
    };
}

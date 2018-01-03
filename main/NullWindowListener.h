#pragma once

#include "IWindowListener.h"

namespace directgraph {
    class NullWindowListener: public IWindowListener {
    public:
        virtual void onClose(void *param);
    };
}



#pragma once

namespace directgraph{
    class IWindowListener{
    public:
        virtual void onClose(void *param) = 0;
        virtual ~IWindowListener(){}
    };
}
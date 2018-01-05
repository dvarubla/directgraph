#pragma once

#include <WException.h>

namespace directgraph {
    class DX9Exception : public WException{
    public:
        DX9Exception(const std::wstring &errStr);
    };
}

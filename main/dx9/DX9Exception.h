#pragma once

#include <WException.h>

namespace directgraph {
    class DX9Exception : public WException{
    public:
        DX9Exception(const wchar_t* filename, uint_fast32_t lineNum, DirectgraphErrorCode code, const std::wstring &errStr);
    };
}

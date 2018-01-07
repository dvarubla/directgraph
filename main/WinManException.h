#pragma once

#include "WException.h"

namespace directgraph {
    class WinManException: public WException {
    public:
        WinManException(
                const wchar_t* filename,
                uint_fast32_t lineNum,
                DirectgraphErrorCode code,
                const std::wstring &errStr,
                DirectgraphWinIndex index
        );
        WinManException(
                const wchar_t* filename,
                uint_fast32_t lineNum,
                DirectgraphErrorCode code,
                const std::wstring &errStr
        );
    };
}



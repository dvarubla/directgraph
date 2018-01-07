#include "DX9Exception.h"

namespace directgraph {
    DX9Exception::DX9Exception(
            const wchar_t* filename, uint_fast32_t lineNum, DirectgraphErrorCode code, const std::wstring &errStr
    ): WException(filename, lineNum, code, errStr) {
    }
}

#include "Exception.h"

namespace directgraph {
    namespace dx9 {
        Exception::Exception(
                const wchar_t *filename, uint_fast32_t lineNum, DirectgraphErrorCode code, const std::wstring &errStr
        ) : WException(filename, lineNum, code, errStr) {
        }
    }
}

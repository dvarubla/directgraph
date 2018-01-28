#pragma once

#include <WException.h>

namespace directgraph {
    namespace dx9 {
        class Exception : public WException {
        public:
            Exception(const wchar_t *filename, uint_fast32_t lineNum, DirectgraphErrorCode code,
                      const std::wstring &errStr);
        };
    }
}

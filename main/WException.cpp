#include "WException.h"

namespace directgraph {
    WException::WException(const std::wstring &errStr)
            : std::runtime_error(""), _errStr(errStr) {
    }

    std::wstring WException::getString() const {
        return _errStr;
    }

    void WException::setString(const std::wstring &str) {
        _errStr = str;
    }
}
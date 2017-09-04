#include "DX9Exception.h"

namespace directgraph {
    DX9Exception::DX9Exception(const std::wstring &errStr) : WException(errStr) {
    }
}

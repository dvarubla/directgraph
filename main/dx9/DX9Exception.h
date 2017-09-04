#ifndef DIRECTGRAPH_DX9EXCEPTION_H
#define DIRECTGRAPH_DX9EXCEPTION_H

#include <WException.h>

namespace directgraph {
    class DX9Exception : public WException{
    public:
        DX9Exception(const std::wstring &errStr);
    };
}

#endif //DIRECTGRAPH_DX9EXCEPTION_H

#pragma once

#include <directgraph/directgraph_constants.h>

namespace directgraph{
#define EXC_CALL_WRAP(code) \
    EXC_CALL_WRAP_EXTRA(code){}

#define EXC_CALL_WRAP_EXTRA(code) \
    bool isCallOk = true;\
    try { code } catch(const WException &ex){\
        std::wstring str = ex.getString();\
        getCurrentErrorHandler()(ex.getFilename(), ex.getLineNum(), ex.getErrCode(), str.c_str());\
        isCallOk = false;\
    }\
    if(!isCallOk)

    DirectgraphErrorHandler getCurrentErrorHandler();
}

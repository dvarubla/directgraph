#pragma once

#include <stdexcept>
#include <directgraph/directgraph_constants.h>
#include <windows.h>

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#define THROW_EXC(excClass, ...) throw excClass(WFILE, __LINE__, __VA_ARGS__)
#define THROW_EXC_CODE(excClass, code, ...) THROW_EXC(excClass, DIRECTGRAPH_##code, __VA_ARGS__)

namespace directgraph {
    class WException : public std::runtime_error {
    private:
        const wchar_t *_filename;
        uint_fast32_t _lineNum;
        DirectgraphErrorCode _errCode;
        std::wstring _errStr;
    public:
        WException(const wchar_t* filename, uint_fast32_t lineNum, DirectgraphErrorCode code, const std::wstring &errStr);

        std::wstring getString() const;

        const wchar_t* getFilename() const;

        DirectgraphErrorCode getErrCode() const;

        uint32_t getLineNum() const;

        void setString(const std::wstring &str);
    };

    void rethrow_exc_wparam(const MSG &msg);
}

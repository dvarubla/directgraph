#include "WException.h"

namespace directgraph {

    std::wstring WException::getString() const {
        return _errStr;
    }

    void WException::setString(const std::wstring &str) {
        _errStr = str;
    }

    WException::WException(const wchar_t *filename, uint_fast32_t lineNum, DirectgraphErrorCode code, const std::wstring &errStr)
        :std::runtime_error(""), _filename(filename), _lineNum(lineNum), _errCode(code), _errStr(errStr) {
    }

    const wchar_t *WException::getFilename() const {
        return _filename;
    }

    uint32_t WException::getLineNum() const {
        return _lineNum;
    }

    DirectgraphErrorCode WException::getErrCode() const {
        return _errCode;
    }

    void rethrow_exc_wparam(const MSG &msg) {
        if(msg.lParam){
            std::exception_ptr *ptrMem = reinterpret_cast<std::exception_ptr*>(msg.wParam);
            std::exception_ptr ptr = *ptrMem;
            delete ptrMem;
            std::rethrow_exception(ptr);
        }
    }
}
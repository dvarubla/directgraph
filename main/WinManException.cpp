#include <sstream>
#include "WinManException.h"

namespace directgraph{

    WinManException::WinManException(
            const wchar_t *filename, uint_fast32_t lineNum, DirectgraphErrorCode code,
            const std::wstring &errStr, DirectgraphWinIndex index
    ) : WException(filename, lineNum, code, L""){
        std::wstringstream strm;
        strm << errStr << L" (Window index: " << index << L")";
        setString(strm.str());
    }

    WinManException::WinManException(
            const wchar_t *filename, uint_fast32_t lineNum, DirectgraphErrorCode code,
            const std::wstring &errStr
    ) : WException(filename, lineNum, code, errStr) {

    }
}
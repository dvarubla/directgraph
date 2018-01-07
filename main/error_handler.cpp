#include "error_handler.h"

#include <windows.h>
#include <sstream>

namespace directgraph {
    static void defaultErrorHandler(const wchar_t *filename, uint_fast32_t lineNum, DirectgraphErrorCode code,
                                    const wchar_t *str) {
        std::wstringstream strm;
        strm << str << L"\nCode: " << code << L" (" << filename << L":" << lineNum << L")" << "\n\n"
             << "Continue program?";
        std::wstring resStr = strm.str();
        if (MessageBoxW(NULL, resStr.c_str(), L"Error!", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL) {
            std::exit(1);
        };
    }

    DirectgraphErrorHandler getCurrentErrorHandler() {
        return defaultErrorHandler;
    }
}
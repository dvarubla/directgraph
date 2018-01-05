#pragma once

#include <stdexcept>

namespace directgraph {
    class WException : public std::runtime_error {
    private:
        std::wstring _errStr;
    public:
        WException(const std::wstring &errStr);

        std::wstring getString() const;

        void setString(const std::wstring &str);
    };
}

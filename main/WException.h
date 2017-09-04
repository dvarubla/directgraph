#ifndef DIRECTGRAPH_WEXCEPTION_H
#define DIRECTGRAPH_WEXCEPTION_H

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
#endif //DIRECTGRAPH_WEXCEPTION_H

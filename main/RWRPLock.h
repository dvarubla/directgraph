#pragma once

#include <windows.h>
#include <stdint.h>

namespace directgraph {
    class RWRPLock {
    private:
        CRITICAL_SECTION _counterCS;
        CRITICAL_SECTION _writerCS;
        uint_fast32_t _numReaders;
        bool _writerWaiting;
        HANDLE _noReaders;
    public:
        RWRPLock();
        ~RWRPLock();
        void startRead();
        void endRead();
        void startWrite();
        void endWrite();
    };
}



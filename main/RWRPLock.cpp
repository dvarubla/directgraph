#include "RWRPLock.h"

namespace directgraph{

    RWRPLock::RWRPLock(): _numReaders(0), _writerWaiting(false), _noReaders(CreateEvent (NULL, FALSE, FALSE, NULL)) {
        InitializeCriticalSection(&_counterCS);
        InitializeCriticalSection(&_writerCS);
    }

    RWRPLock::~RWRPLock() {
        DeleteCriticalSection(&_counterCS);
        DeleteCriticalSection(&_writerCS);
        CloseHandle(_noReaders);
    }

    void RWRPLock::startRead() {
        EnterCriticalSection(&_counterCS);
        _numReaders++;
        LeaveCriticalSection(&_counterCS);
    }

    void RWRPLock::endRead() {
        EnterCriticalSection(&_counterCS);
        _numReaders--;
        if(_numReaders == 0 && _writerWaiting){
            SetEvent(_noReaders);
        }
        LeaveCriticalSection(&_counterCS);
    }

    void RWRPLock::startWrite() {
        EnterCriticalSection(&_writerCS);
        EnterCriticalSection(&_counterCS);
        _writerWaiting = true;
        while(_numReaders > 0){
            LeaveCriticalSection(&_counterCS);
            WaitForSingleObject(_noReaders, INFINITE);
            EnterCriticalSection(&_counterCS);
        }
        _writerWaiting = false;
    }

    void RWRPLock::endWrite() {
        LeaveCriticalSection(&_counterCS);
        LeaveCriticalSection(&_writerCS);
    }
}
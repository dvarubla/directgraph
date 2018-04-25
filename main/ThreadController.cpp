#include "ThreadController.h"
#include "Queue.h"
#include "QueueItem.h"
#include "WException.h"
#include "patterns.h"

namespace directgraph{

    ThreadController::ThreadController(IMyWindow *window, const CommonProps &props)
            : _window(window), _queue(), _reader(&_queue, &_queueCS),
              _threadStarted(0), _numDrawMsgs(0), _pixContFactory(_window->getRenderer()->getPixContFactory()) {
        _currentProps = props;
        _initialProps = props;
        InitializeCriticalSection(&_addCS);
        InitializeCriticalSection(&_queueCS);
        InitializeCriticalSection(&_propsCS);

        if(_currentProps.userFillPattern == NULL){
            _currentProps.userFillPattern = reinterpret_cast<char *>(new uint8_t[FPATTERN_SIZE]);
        }
    }

    void ThreadController::checkGrow() {
        if(_queue.needGrow()) {
            EnterCriticalSection(&_queueCS);
            _queue.grow();
            LeaveCriticalSection(&_queueCS);
        }
    }

    void ThreadController::sendPrepareMsg() {
        if(InterlockedCompareExchange(&_threadStarted, 0, 0)
           && InterlockedCompareExchange(&_numDrawMsgs, 1, 0) == 0){
            PostThreadMessage(_drawThreadId, PREPARE, 0, 0);
        }
    }

    void ThreadController::flushPixels() {
        if(_pixContFactory->havePixels()){
            for(
                    PixelContainerCreator::PixelVector::iterator it = _pixContFactory->pixelsBegin();
                    it != _pixContFactory->pixelsEnd();
                    ++it
                    ){
                checkGrow();
                _queue.writeItem(QueueItemCreator::create<QueueItem::SINGLE_PIXEL>(it->x, it->y, it->color));
            }
        }
        if(_pixContFactory->haveContainer()){
            checkGrow();
            _queue.writeItem(QueueItemCreator::create<QueueItem::PIXEL_CONTAINER>(_pixContFactory->getContainer()));
        }
        _pixContFactory->clear();
    }

    void ThreadController::writeItemHelper(const QueueItem &item) {
        EnterCriticalSection(&_addCS);
        writeItemHelperNoLock(item);
    }

    void ThreadController::writeItemHelperNoLock(const QueueItem &item) {
        flushPixels();
        checkGrow();
        _queue.writeItem(item);
        LeaveCriticalSection(&_addCS);
        sendPrepareMsg();
    }

    void ThreadController::writeItemHelperColor(const QueueItem &item, uint_fast32_t color) {
        flushPixels();
        checkGrow();
        _queue.writeItem(QueueItemCreator::create<QueueItem::COLOR>(_paletteMan.getColor(color)));
        checkGrow();
        _queue.writeItem(item);
        checkGrow();
        _queue.writeItem(QueueItemCreator::create<QueueItem::COLOR>(_paletteMan.getColor(_currentProps.drawColor)));
        LeaveCriticalSection(&_propsCS);
        LeaveCriticalSection(&_addCS);
        sendPrepareMsg();
    }

    void ThreadController::clearAndReset() {
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        flushPixels();
        checkGrow();
        char *pattern = _currentProps.userFillPattern;
        _currentProps = _initialProps;
        _currentProps.userFillPattern = pattern;
        _queue.writeItem(QueueItemCreator::create<QueueItem::CLEAR>());
        _queue.writeItem(QueueItemCreator::create<QueueItem::COLOR>(_currentProps.drawColor));
        _queue.writeItem(QueueItemCreator::create<QueueItem::BGCOLOR>(_currentProps.bgColor));
        _queue.writeItem(QueueItemCreator::create<QueueItem::SETFILLSTYLE>(_currentProps.fillStyle, _currentProps.fillColor));
        _queue.writeItem(QueueItemCreator::create<QueueItem::SETLINESTYLE>(
                _currentProps.lineStyle, _currentProps.userLinePattern, _currentProps.lineThickness)
        );
        LeaveCriticalSection(&_propsCS);
        LeaveCriticalSection(&_addCS);
    }

    void ThreadController::clear() {
        QueueItem item = QueueItemCreator::create<QueueItem::CLEAR>();
        writeItemHelper(item);
    }

    void ThreadController::fillellipse(
            int_fast32_t x, int_fast32_t y, uint_fast32_t xradius, uint_fast32_t yradius,
            bool useColor, uint_fast32_t color
    ) {
        if(!_paramsChecker.checkEllipse(x, y, xradius, yradius)){
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::FILLELLIPSE>(x, y, xradius, yradius);
        if(useColor) {
            EnterCriticalSection(&_addCS);
            EnterCriticalSection(&_propsCS);
            writeItemHelperColor(item, color);
        } else {
            writeItemHelper(item);
        }
    }

    void ThreadController::setlinestyle(uint_fast8_t linestyle, uint_fast16_t pattern, uint_fast32_t thickness) {
        _paramsChecker.checkLineStyle(linestyle);
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        _currentProps.lineStyle = linestyle;
        _currentProps.userLinePattern = pattern;
        _currentProps.lineThickness = thickness;
        LeaveCriticalSection(&_propsCS);
        QueueItem item = QueueItemCreator::create<QueueItem::SETLINESTYLE>(linestyle, pattern, thickness);
        writeItemHelperNoLock(item);
    }

    void ThreadController::bar(int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom) {
        if(
                !_paramsChecker.checkBar(left, top, right, bottom)
        ){
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::BAR>(left, top, right, bottom);
        writeItemHelper(item);
    }

    void ThreadController::drawpoly(
            uint_fast32_t numPoints, const int32_t *points,
            bool useColor, uint_fast32_t color
    ) {
        EnterCriticalSection(&_propsCS);
        if(_currentProps.lineStyle == NULL_LINE){
            LeaveCriticalSection(&_propsCS);
            return;
        }
        if(numPoints <= 1){
            LeaveCriticalSection(&_propsCS);
            return;
        }
        if(numPoints == 2){
            LeaveCriticalSection(&_propsCS);
            line(points[0], points[1], points[2], points[3], useColor, color);
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::DRAWPOLY>(numPoints, points);
        if(useColor){
            EnterCriticalSection(&_addCS);
            writeItemHelperColor(item, color);
        } else {
            LeaveCriticalSection(&_propsCS);
            writeItemHelper(item);
        }
    }

    void ThreadController::ellipse(
            int_fast32_t x, int_fast32_t y, uint_fast32_t xradius, uint_fast32_t yradius,
            bool useColor, uint_fast32_t color
    ) {
        if(!_paramsChecker.checkEllipse(x, y, xradius, yradius)){
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::ELLIPSE>(x, y, xradius, yradius);
        if(useColor) {
            EnterCriticalSection(&_addCS);
            EnterCriticalSection(&_propsCS);
            writeItemHelperColor(item, color);
        } else {
            writeItemHelper(item);
        }
    }

    void ThreadController::bar3d(
            int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom,
            uint_fast32_t depth, bool haveTop, bool useColor, uint_fast32_t color
    ) {
        if(
                !_paramsChecker.checkBar(left, top, right, bottom)
                ){
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::BAR3D>(left, top, right, bottom, depth, haveTop);
        if(useColor) {
            EnterCriticalSection(&_addCS);
            EnterCriticalSection(&_propsCS);
            writeItemHelperColor(item, color);
        } else {
            writeItemHelper(item);
        }
    }

    void ThreadController::fillpoly(
            uint_fast32_t numPoints, const int32_t *points,
            bool useColor, uint_fast32_t color
    ) {
        if(numPoints <= 1){
            return;
        }
        if(numPoints == 2){
            line(points[0], points[1], points[2], points[3], useColor, color);
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::FILLPOLY>(numPoints, points);
        if(useColor){
            EnterCriticalSection(&_addCS);
            EnterCriticalSection(&_propsCS);
            writeItemHelperColor(item, color);
        } else {
            writeItemHelper(item);
        }
    }

    void ThreadController::setfillstyle(uint_fast8_t fillStyle, uint_fast32_t color) {
        _paramsChecker.checkFillStyle(fillStyle);
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        _currentProps.fillStyle = fillStyle;
        _currentProps.fillColor = color;
        color = _paletteMan.getColor(color);
        LeaveCriticalSection(&_propsCS);
        QueueItem item = QueueItemCreator::create<QueueItem::SETFILLSTYLE>(fillStyle, color);
        writeItemHelperNoLock(item);
    }

    void ThreadController::setbgcolor(uint_fast32_t color) {
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        _currentProps.bgColor = color;
        color = _paletteMan.getColor(color);
        LeaveCriticalSection(&_propsCS);
        QueueItem item = QueueItemCreator::create<QueueItem::BGCOLOR>(color);
        writeItemHelperNoLock(item);
    }

    void ThreadController::setcolor(uint_fast32_t color) {
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        _currentProps.drawColor = color;
        color = _paletteMan.getColor(color);
        LeaveCriticalSection(&_propsCS);
        QueueItem item = QueueItemCreator::create<QueueItem::COLOR>(color);
        writeItemHelperNoLock(item);
    }

    void ThreadController::setfillpattern(const char *fillpattern, uint_fast32_t color) {
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        std::copy(fillpattern, fillpattern + FPATTERN_SIZE, _currentProps.userFillPattern);
        _currentProps.fillStyle = USER_FILL;
        _currentProps.fillColor = color;
        color = _paletteMan.getColor(color);
        LeaveCriticalSection(&_propsCS);
        QueueItem item = QueueItemCreator::create<QueueItem::SETFILLPATTERN>(fillpattern, color);
        writeItemHelperNoLock(item);
    }

    void ThreadController::rectangle(
            int_fast32_t left, int_fast32_t top, int_fast32_t right, int_fast32_t bottom,
            bool useColor, uint_fast32_t color
    ){
        EnterCriticalSection(&_propsCS);
        if(
                !_paramsChecker.checkRectangle(left, top, right, bottom, _currentProps.lineStyle)
        ){
            LeaveCriticalSection(&_propsCS);
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::RECTANGLE>(left, top, right, bottom);
        if(useColor){
            EnterCriticalSection(&_addCS);
            writeItemHelperColor(item, color);
        } else {
            LeaveCriticalSection(&_propsCS);
            writeItemHelper(item);
        }
    }

    void ThreadController::putpixel(int_fast32_t x, int_fast32_t y, uint_fast32_t color) {
        if(
            !_paramsChecker.checkPixel(x, y, _pixContFactory->getMaxWidth(), _pixContFactory->getMaxHeight())
        ){
            return;
        }

        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        _pixContFactory->addPixel(static_cast<uint_fast32_t>(x), static_cast<uint_fast32_t>(y),
                                  color_remove_alpha(_paletteMan.getColor(color)));
        LeaveCriticalSection(&_propsCS);
        LeaveCriticalSection(&_addCS);
    }

    void ThreadController::putpixel(int_fast32_t x, int_fast32_t y) {
        if(
                !_paramsChecker.checkPixel(x, y, _pixContFactory->getMaxWidth(), _pixContFactory->getMaxHeight())
                ){
            return;
        }

        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        _pixContFactory->addPixel(static_cast<uint_fast32_t>(x), static_cast<uint_fast32_t>(y),
                                  color_remove_alpha(_paletteMan.getColor(_currentProps.drawColor)));
        LeaveCriticalSection(&_propsCS);
        LeaveCriticalSection(&_addCS);
    }

    void ThreadController::repaint() {
        EnterCriticalSection(&_addCS);
        flushPixels();
        LeaveCriticalSection(&_addCS);

        while(!InterlockedCompareExchange(&_threadStarted, 0, 0)){
            Sleep(100);
        }
        MSG msg;
        InterlockedIncrement(&_numDrawMsgs);
        PostThreadMessage(_drawThreadId, REPAINT, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, REPLY, REPLY);
        rethrow_exc_wparam(msg);
    }

    void ThreadController::repaintThread() {
        MSG msg;
        PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
        InterlockedExchange(&_threadStarted, 1);
        while(true) {
            GetMessage(&msg, NULL, 0, 0);
            if(msg.message == STOP){
                PostThreadMessage(static_cast<DWORD>(msg.wParam), REPLY, 0, 0);
                break;
            }
            while(true) {
                EnterCriticalSection(&_queueCS);
                uint_fast32_t readSize = _queue.getReadSize();
                if(readSize == 0 && msg.message == PREPARE){
                    LeaveCriticalSection(&_queueCS);
                    break;
                }
                _reader.setGetIndex(_queue.getGetIndex(), readSize);
                if(msg.message == REPAINT) {
                    try {
                        _window->getRenderer()->draw(&_reader);
                    } catch (const std::exception &) {
                        LeaveCriticalSection(&_queueCS);
                        std::exception_ptr *ptrMem = new std::exception_ptr;
                        *ptrMem = std::current_exception();
                        PostThreadMessage(
                                static_cast<DWORD>(msg.wParam), REPLY,
                                reinterpret_cast<WPARAM>(ptrMem), true
                        );
                        break;
                    }
                } else {
                    _window->getRenderer()->prepare(&_reader);
                    break;
                }
                if(readSize == 0){
                    break;
                }
            }
            InterlockedDecrement(&_numDrawMsgs);
            if(msg.message == REPAINT) {
                _window->getRenderer()->repaint();
                PostThreadMessage(static_cast<DWORD>(msg.wParam), REPLY, 0, 0);
            }
        }
    }

    void ThreadController::stopRepaintThread() {
        while(!InterlockedCompareExchange(&_threadStarted, 0, 0)){
            Sleep(100);
        }
        MSG msg;
        PostThreadMessage(_drawThreadId, STOP, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, REPLY, REPLY);
        WaitForSingleObject(_drawThread, INFINITE);
        CloseHandle(_drawThread);
    }

    ThreadController::~ThreadController() {
        stopRepaintThread();
        DeleteCriticalSection(&_addCS);
        DeleteCriticalSection(&_queueCS);
        DeleteCriticalSection(&_propsCS);
        delete [] _currentProps.userFillPattern;
    }

    DWORD WINAPI repaintFunc(LPVOID ctrl){
        ((ThreadController*)ctrl)->repaintThread();
        return 0;
    }

    void ThreadController::init() {
        clear();
        _drawThread = CreateThread(NULL, 0, repaintFunc, (LPVOID)this, 0, &_drawThreadId);
    }

    uint_fast32_t ThreadController::getcolor() {
        uint_fast32_t color;
        EnterCriticalSection(&_propsCS);
        color = _currentProps.drawColor;
        LeaveCriticalSection(&_propsCS);
        return color;
    }

    uint_fast32_t ThreadController::getbgcolor() {
        uint_fast32_t color;
        EnterCriticalSection(&_propsCS);
        color = _currentProps.bgColor;
        LeaveCriticalSection(&_propsCS);
        return color;
    }

    void ThreadController::getlinesettings(linesettingstype *lineinfo) {
        EnterCriticalSection(&_propsCS);
        lineinfo->thickness = _currentProps.lineThickness;
        lineinfo->linestyle = _currentProps.lineStyle;
        lineinfo->upattern = _currentProps.userLinePattern;
        LeaveCriticalSection(&_propsCS);
    }

    void ThreadController::getfillsettings(fillsettingstype *fillinfo) {
        EnterCriticalSection(&_propsCS);
        fillinfo->color = _currentProps.fillColor;
        fillinfo->pattern = _currentProps.fillStyle;
        LeaveCriticalSection(&_propsCS);
    }

    void ThreadController::getfillpattern(char *pattern) {
        EnterCriticalSection(&_propsCS);
        std::copy(_currentProps.userFillPattern, _currentProps.userFillPattern + FPATTERN_SIZE, pattern);
        LeaveCriticalSection(&_propsCS);
    }

    void ThreadController::line(
            int_fast32_t startx, int_fast32_t starty, int_fast32_t endx, int_fast32_t endy,
            bool useColor, uint_fast32_t color
    ) {
        EnterCriticalSection(&_propsCS);
        if(_currentProps.lineStyle == NULL_LINE){
            LeaveCriticalSection(&_propsCS);
            return;
        }
        QueueItem item = QueueItemCreator::create<QueueItem::LINE>(startx, starty, endx, endy);
        if(useColor){
            EnterCriticalSection(&_addCS);
            writeItemHelperColor(item, color);
        } else {
            LeaveCriticalSection(&_propsCS);
            writeItemHelper(item);
        }
    }

    void ThreadController::lineto(
            int_fast32_t x, int_fast32_t y,
            bool useColor, uint_fast32_t color
    ){
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        if(_currentProps.lineStyle == NULL_LINE){
            LeaveCriticalSection(&_propsCS);
            LeaveCriticalSection(&_addCS);
            return;
        }
        int_fast32_t lastx = _currentProps.curPos.x;
        int_fast32_t lasty = _currentProps.curPos.y;
        _currentProps.curPos.x = x;
        _currentProps.curPos.y = y;
        QueueItem item = QueueItemCreator::create<QueueItem::LINE>(lastx, lasty, x, y);
        if(useColor){
            writeItemHelperColor(item, color);
        } else {
            LeaveCriticalSection(&_propsCS);
            writeItemHelperNoLock(item);
        }
    }

    void ThreadController::linerel(
            int_fast32_t x, int_fast32_t y,
            bool useColor, uint_fast32_t color
    ) {
        EnterCriticalSection(&_addCS);
        EnterCriticalSection(&_propsCS);
        if(_currentProps.lineStyle == NULL_LINE){
            LeaveCriticalSection(&_propsCS);
            LeaveCriticalSection(&_addCS);
            return;
        }
        int_fast32_t lastx = _currentProps.curPos.x;
        int_fast32_t lasty = _currentProps.curPos.y;
        _currentProps.curPos.x += x;
        _currentProps.curPos.y += y;
        QueueItem item = QueueItemCreator::create<QueueItem::LINE>(lastx, lasty, lastx + x, lasty + y);
        if(useColor){
            writeItemHelperColor(item, color);
        } else {
            LeaveCriticalSection(&_propsCS);
            writeItemHelperNoLock(item);
        }
    }

    void ThreadController::moveto(int_fast32_t x, int_fast32_t y) {
        EnterCriticalSection(&_propsCS);
        _currentProps.curPos.x = x;
        _currentProps.curPos.y = y;
        LeaveCriticalSection(&_propsCS);
    }

    void ThreadController::moverel(int_fast32_t x, int_fast32_t y) {
        EnterCriticalSection(&_propsCS);
        _currentProps.curPos.x += x;
        _currentProps.curPos.y += y;
        LeaveCriticalSection(&_propsCS);
    }

    int_fast32_t ThreadController::getx() {
        EnterCriticalSection(&_propsCS);
        int_fast32_t x = _currentProps.curPos.x;
        LeaveCriticalSection(&_propsCS);
        return x;
    }

    int_fast32_t ThreadController::gety() {
        EnterCriticalSection(&_propsCS);
        int_fast32_t y = _currentProps.curPos.y;
        LeaveCriticalSection(&_propsCS);
        return y;
    }

    void ThreadController::initpalette(bool havePalette, uint_fast32_t size, bool fillFirstColors) {
        EnterCriticalSection(&_propsCS);
        _paletteMan.initPalette(havePalette, size, fillFirstColors);
        LeaveCriticalSection(&_propsCS);
    }

    void ThreadController::setpalette(uint_fast32_t index, uint_fast32_t color) {
        EnterCriticalSection(&_propsCS);
        _paletteMan.setColor(index, color);
        LeaveCriticalSection(&_propsCS);
    }

    void ThreadController::clearpalette() {
        EnterCriticalSection(&_propsCS);
        _paletteMan.clearPalette();
        LeaveCriticalSection(&_propsCS);
    }

    uint_fast32_t ThreadController::getmaxx() {
        uint_fast32_t width;
        EnterCriticalSection(&_propsCS);
        width = _window->getWidth();
        LeaveCriticalSection(&_propsCS);
        return width;
    }

    uint_fast32_t ThreadController::getmaxy() {
        uint_fast32_t height;
        EnterCriticalSection(&_propsCS);
        height = _window->getHeight();
        LeaveCriticalSection(&_propsCS);
        return height;
    }
}
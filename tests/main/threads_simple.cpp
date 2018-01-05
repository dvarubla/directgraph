#include <gtest/gtest.h>
#include <main/ThreadController.h>
#include <graphics_const_internal.h>
#include <RendererStub.h>
#include <tests/stubs/QueueItemUtils.h>
#include <tests/stubs/MyWindowStub.h>

using namespace directgraph;
using testing::Return;
using testing::Invoke;
using testing::WithArgs;
using testing::NiceMock;
using testing::_;

class ThreadsSimpleTest : public ::testing::Test {
public:
    MyWindowStub *win;
    RendererStub *ren;

    void addPointsLine(
            ThreadController &tc,
            std::vector<QueueItem> &items,
            uint_fast32_t line,
            uint_fast32_t &cnt
    ){
        QueueItem item;
        for(uint_fast32_t j = 0; j < 200; j++, cnt++){
            item.type = QueueItem::SETFILLSTYLE;
            item.data.setfillstyle = {SOLID_FILL, cnt % 0xFF};
            items.push_back(item);
            tc.setfillstyle(SOLID_FILL, cnt % 0xFF);
            item.type = QueueItem::BAR;
            item.data.bar = {(float)j, (float)line, (float)j + 1, (float)line + 1};
            items.push_back(item);
            tc.bar((float)j, (float)line, (float)j + 1, (float)line + 1);
            if(cnt % 10 == 0){
                Sleep(rand() % 5);
            }
        }
    }

    void addPoints(ThreadController &tc, std::vector<QueueItem> &items){
        uint_fast32_t cnt = 0;
        QueueItem item;
        item.type = QueueItem::CLEAR;
        items.push_back(item);
        for(uint_fast32_t i = 0; i < 20; i++){
            addPointsLine(tc, items, i, cnt);
        }
    }
protected:
    ThreadsSimpleTest() {
        ren = new NiceMock<RendererStub>();
        win = new NiceMock<MyWindowStub>();
        ON_CALL(*win, getRenderer()).WillByDefault(Return(ren));
        ON_CALL(*ren, draw(_, _)).WillByDefault(testing::WithArgs<0>(Invoke(ren, &RendererStub::drawImpl)));
    }

    virtual ~ThreadsSimpleTest() {
        delete ren;
        delete win;
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};


static const int MSG_CODE = WM_USER + 6000;

TEST_F(ThreadsSimpleTest, BarOneThread){
    ThreadController tc(win);
    tc.init();
    std::vector<QueueItem> items;
    addPoints(tc, items);
    tc.repaint();
    EXPECT_EQ(ren->getData(), items);
}

struct RepParam{
    ThreadController *controller;
    uint_fast32_t maxSleep;
    DWORD id;
};

struct DrawParam{
    ThreadController *controller;
    ThreadsSimpleTest *test;
    std::vector<QueueItem> *items;
    uint_fast32_t line;
    DWORD id;
};

static DWORD WINAPI BarRepHelper(LPVOID param){
    RepParam *p = static_cast<RepParam *>(param);
    PostThreadMessage(p->id, MSG_CODE, 0, 0);
    while(true){
        MSG msg;
        if(PeekMessage(&msg, NULL, MSG_CODE, MSG_CODE, PM_REMOVE)){
            PostThreadMessage(static_cast<DWORD>(msg.wParam), MSG_CODE, 0, 0);
            break;
        }
        p->controller->repaint();
        Sleep(rand() % p->maxSleep);
    }
    return 0;
}

TEST_F(ThreadsSimpleTest, BarOneDrawOneRepLong){
    ThreadController tc(win);
    tc.init();
    DWORD threadId;
    MSG msg;
    RepParam p = {&tc, 30, GetCurrentThreadId()};
    CreateThread(NULL, 0, BarRepHelper, &p, 0, &threadId);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);

    std::vector<QueueItem> items;
    addPoints(tc, items);
    tc.repaint();
    PostThreadMessage(threadId, MSG_CODE, GetCurrentThreadId(), 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    EXPECT_EQ(ren->getData(), items);
}

TEST_F(ThreadsSimpleTest, BarOneDrawOneRepShort){
    ThreadController tc(win);
    tc.init();
    DWORD threadId;
    MSG msg;
    RepParam p = {&tc, 2, GetCurrentThreadId()};
    CreateThread(NULL, 0, BarRepHelper, &p, 0, &threadId);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);

    std::vector<QueueItem> items;
    addPoints(tc, items);
    tc.repaint();
    PostThreadMessage(threadId, MSG_CODE, GetCurrentThreadId(), 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    EXPECT_EQ(ren->getData(), items);
}

TEST_F(ThreadsSimpleTest, BarOneDrawMultRep){
    ThreadController tc(win);
    tc.init();
    const int NUM_THREADS = 3;
    DWORD threadIds[NUM_THREADS];
    RepParam params[NUM_THREADS];
    MSG msg;
    for(int i = 0; i < NUM_THREADS; i++){
        params[i] = {&tc, static_cast<uint_fast32_t>(rand() % 20 + 1), GetCurrentThreadId()};
        CreateThread(NULL, 0, BarRepHelper, &params[i], 0, &threadIds[i]);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    std::vector<QueueItem> items;
    addPoints(tc, items);
    for(int i = 0; i < NUM_THREADS; i++){
        PostThreadMessage(threadIds[i], MSG_CODE, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    tc.repaint();
    ASSERT_EQ(ren->getData().size(), items.size());
    EXPECT_EQ(ren->getData(), items);
}

CRITICAL_SECTION crit;

static DWORD WINAPI BarDrawHelper(LPVOID param){
    DrawParam *p = static_cast<DrawParam *>(param);
    PostThreadMessage(p->id, MSG_CODE, 0, 0);
    uint_fast32_t cnt = 0;
    {
        std::vector<QueueItem> localItems;
        p->test->addPointsLine(*p->controller, localItems, p->line, cnt);

        EnterCriticalSection(&crit);
        std::copy(localItems.cbegin(), localItems.cend(), std::back_inserter(*p->items));
        LeaveCriticalSection(&crit);
    }
    MSG msg;
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    PostThreadMessage(static_cast<DWORD>(msg.wParam), MSG_CODE, 0, 0);
    return 0;
}

TEST_F(ThreadsSimpleTest, BarMultDrawMultRep){
    InitializeCriticalSection(&crit);
    ThreadController tc(win);
    tc.init();
    const int NUM_REP_THREADS = 5;
    DWORD repThreadIds[NUM_REP_THREADS];
    RepParam repParams[NUM_REP_THREADS];
    MSG msg;

    std::vector<QueueItem> items;
    QueueItem item;
    item.type = QueueItem::CLEAR;
    items.push_back(item);

    for(int i = 0; i < NUM_REP_THREADS; i++){
        repParams[i] = {&tc, static_cast<uint_fast32_t>(rand() % 20 + 1), GetCurrentThreadId()};
        CreateThread(NULL, 0, BarRepHelper, &repParams[i], 0, &repThreadIds[i]);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    const int NUM_DRAW_THREADS = 4;
    DWORD drawThreadIds[NUM_DRAW_THREADS];
    DrawParam drawParams[NUM_DRAW_THREADS];
    for(int i = 0; i < NUM_DRAW_THREADS; i++){
        drawParams[i] = {&tc, this, &items, static_cast<uint_fast32_t>(i), GetCurrentThreadId()};
        CreateThread(NULL, 0, BarDrawHelper, &drawParams[i], 0, &drawThreadIds[i]);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    for(int i = 0; i < NUM_DRAW_THREADS; i++){
        PostThreadMessage(drawThreadIds[i], MSG_CODE, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    for(int i = 0; i < NUM_REP_THREADS; i++){
        PostThreadMessage(repThreadIds[i], MSG_CODE, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    tc.repaint();
    DeleteCriticalSection(&crit);
    std::sort(ren->getData().begin(), ren->getData().end(), Comparator());
    std::sort(items.begin(), items.end(), Comparator());
    ASSERT_EQ(ren->getData().size(), items.size());
    EXPECT_EQ(ren->getData(), items);
}
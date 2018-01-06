#include <gtest/gtest.h>
#include <main/ThreadController.h>
#include <graphics_const_internal.h>
#include <RendererStub.h>
#include <tests/stubs/MyWindowStub.h>
#include <tests/stubs/QueueItemUtils.h>
#include <QueueItem.h>
#include <PixelContainer.h>

using namespace directgraph;
using testing::Return;
using testing::Invoke;
using testing::WithArgs;
using testing::NiceMock;
using testing::_;

class ThreadsLastTest : public ::testing::Test {
public:
    MyWindowStub *win;
    RendererStub *ren;
    PixelContainerFactory *fact;

    void addPoints(ThreadController &tc, std::vector<QueueItem> &items){
        uint_fast32_t cnt = 0;
        QueueItem item;
        item.type = QueueItem::CLEAR;
        items.push_back(item);
        item.type = QueueItem::PIXEL_CONTAINER;
        item.data.pixelContainer = new PixelContainer<IPixelContainer::R8G8B8>(5, 2, 0, 6, 2, 0, 500, 500);
        tc.putpixel(5, 2, 0);
        tc.putpixel(6, 2, 0);
        items.push_back(item);
        for(uint_fast32_t i = 2; i <= 25; i++){
            for(uint_fast32_t j = (i == 2) ? 7: 5; j <= 200; j++, cnt++){
                item.data.pixelContainer->tryAddPixel(j, i, 0);
                if(cnt % 10 == 0){
                    Sleep(rand() % 5);
                }
                tc.putpixel(j, i, 0);
            }
        }
    }

    uint_fast32_t countTotal(const std::vector<QueueItem> &items){
        uint_fast32_t size = 0;
        for(uint_fast32_t i = 0; i < items.size(); i++){
            if(items[i].type == QueueItem::PIXEL_CONTAINER){
                IPixelContainer *cont = items[i].data.pixelContainer;
                directgraph::Rectangle firstCoords = cont->getFirstCoords();
                size += (firstCoords.right - firstCoords.left + 1) * (firstCoords.bottom - firstCoords.top + 1);
                if(cont->getStride() != cont->getLastStride()){
                    size += (cont->getLastCoords().right - cont->getLastCoords().left + 1);
                }
                delete cont;
            } else if(items[i].type == QueueItem::SINGLE_PIXEL){
                size++;
            }
        }
        return size;
    }
protected:
    ThreadsLastTest() {
        ren = new NiceMock<RendererStub>();
        fact = new PixelContainerFactory(500, 500, IPixelContainer::R8G8B8);
        win = new NiceMock<MyWindowStub>();
        ON_CALL(*win, getRenderer()).WillByDefault(Return(ren));
        ON_CALL(*ren, draw(_, _)).WillByDefault(testing::WithArgs<0>(Invoke(ren, &RendererStub::drawImpl)));
        ON_CALL(*ren, getPixContFactory()).WillByDefault(Return(fact));
    }

    virtual ~ThreadsLastTest() {
        delete ren;
        delete fact;
        delete win;
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(ThreadsLastTest, ContainerOneThread){
    ThreadController tc(win);
    tc.init();
    std::vector<QueueItem> items;
    addPoints(tc, items);
    tc.repaint();
    EXPECT_EQ(ren->getData(), items);
}

static const int MSG_CODE = WM_USER + 6000;

struct RepParam{
    ThreadController *controller;
    uint_fast32_t maxSleep;
    DWORD id;
};

static DWORD WINAPI ContainerRepHelper(LPVOID param){
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

TEST_F(ThreadsLastTest, ContainerOneDrawOneRep){
    ThreadController tc(win);
    tc.init();
    DWORD threadId;
    MSG msg;
    RepParam p = {&tc, 5, GetCurrentThreadId()};
    CreateThread(NULL, 0, ContainerRepHelper, &p, 0, &threadId);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    std::vector<QueueItem> items;
    addPoints(tc, items);
    PostThreadMessage(threadId, MSG_CODE, GetCurrentThreadId(), 0);
    GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    tc.repaint();
    EXPECT_EQ(countTotal(ren->getData()), countTotal(items));
}

TEST_F(ThreadsLastTest, ContainerOneDrawMultRep){
    ThreadController tc(win);
    tc.init();
    const int NUM_THREADS = 3;
    DWORD threadIds[NUM_THREADS];
    RepParam params[NUM_THREADS];
    MSG msg;
    for(int i = 0; i < NUM_THREADS; i++){
        RepParam p = {&tc, 5, GetCurrentThreadId()};
        params[i] = p;
        CreateThread(NULL, 0, ContainerRepHelper, &params[i], 0, &threadIds[i]);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    std::vector<QueueItem> items;
    addPoints(tc, items);
    for(int i = 0; i < NUM_THREADS; i++){
        PostThreadMessage(threadIds[i], MSG_CODE, GetCurrentThreadId(), 0);
        GetMessage(&msg, NULL, MSG_CODE, MSG_CODE);
    }

    tc.repaint();
    EXPECT_EQ(countTotal(ren->getData()), countTotal(items));
}
#include <gtest/gtest.h>
#include "CommonIntegration.h"
#include <testlib/testlib_funcs.h>

class DX9Int1Test: public ImageTest, public CommonIntegration{
public:
    static uint32_t WIDTH;
    static uint32_t HEIGHT;
    DirectgraphWinIndex _curIndex;
    DX9Int1Test(){
        initWindowManager();
        createWindow();
    }

    void createWindow(){
        DirectgraphWinParams p;
        p.name = L"Directgraph";
        p.width = WIDTH;
        p.height = HEIGHT;
        p.renderer = DIRECTGRAPH_DX9_RENDERER;
        p.controller = DIRECTGRAPH_MULT_THREAD_CTRL;
        _curIndex = wman->createWindow(p);
    }

    void drawGrid(IController *ctrl){
        ctrl->setfillstyle(SOLID_FILL, 0x994444);
        ctrl->bar(0, 0, WIDTH, HEIGHT);
        ctrl->setfillstyle(SOLID_FILL, 0xFFFFFF);
        for(uint_fast32_t i = 0; i < HEIGHT/10; i++){
            for(uint_fast32_t j = 0; j < WIDTH/10; j++){
                ctrl->bar(
                        static_cast<int_fast32_t>(j * 10 + 1.0f),
                        static_cast<int_fast32_t>(i * 10 + 1.0f),
                        static_cast<int_fast32_t>((j + 1) * 10.0f),
                        static_cast<int_fast32_t>((i + 1) * 10.0f)
                );
            }
        }
    }
};

uint32_t DX9Int1Test::WIDTH = 561;
uint32_t DX9Int1Test::HEIGHT = 401;

IMG_TEST_F(DX9Int1Test, Vehicle){
    IController *ctrl = wman->getCurrentWindowAndLock().controller;
    drawGrid(ctrl);
    ctrl->setlinestyle(NULL_LINE, 0, 0);
    ctrl->setbgcolor(0xAAAAAA);
    ctrl->setfillstyle(SLASH_FILL, 0x444444);
    ctrl->bar(50, 190, 70, 210);
    ctrl->bar(270, 100, 370, 120);
    ctrl->bar(490, 210, 510, 190);

    //Blue
    ctrl->setfillstyle(SOLID_FILL, 0xFF4422);
    ctrl->bar(70, 150, 150, 210);
    ctrl->fillellipse(210, 180, 60, 40);
    ctrl->setbgcolor(0xCCCCCC);
    uint8_t ptrn[] = {
            0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    ctrl->setfillpattern(reinterpret_cast<char *>(ptrn), 0xFF4422);
    ctrl->bar(190, 80, 250, 140);

    //Wheels
    ctrl->setfillstyle(SOLID_FILL, 0x0000FF);
    ctrl->fillellipse(160, 250, 40, 40);
    ctrl->fillellipse(410, 250, 40, 40);

    ctrl->setfillstyle(SOLID_FILL, 0xFFFFFF);
    ctrl->fillellipse(160, 250, 10, 10);
    ctrl->fillellipse(410, 250, 10, 10);

    for(uint_fast32_t i = 155; i < 166; i++){
        ctrl->putpixel(i, 250, 0);
    }

    for(uint_fast32_t i = 245; i < 256; i++){
        ctrl->putpixel(160, i, 0);
    }

    //Middle
    ctrl->setfillstyle(SOLID_FILL, 0x00FF22);
    ctrl->bar(250, 60, 270, 140);
    ctrl->bar(270, 120, 370, 240);

    ctrl->setfillstyle(SOLID_FILL, 0x00BBFF);
    ctrl->bar(370, 110, 490, 210);

    for(uint_fast32_t i = 405; i < 416; i++){
        ctrl->putpixel(i, 250, 0);
    }

    for(uint_fast32_t i = 245; i < 256; i++){
        ctrl->putpixel(410, i, 0);
    }

    ctrl->repaint();
    wman->releaseCurrentWindowLock();
    return afterTestSimple(_curIndex);
}

IMG_TEST_F(DX9Int1Test, Tractor){
    IController *ctrl = wman->getCurrentWindowAndLock().controller;
    drawGrid(ctrl);

    //First block
    ctrl->setlinestyle(SOLID_LINE, 0, 10);
    ctrl->setcolor(0);
    ctrl->rectangle(325, 235, 135, 165);

    ctrl->setbgcolor(0x0044AA);
    ctrl->setfillstyle(LINE_FILL, 0x00FFFF);
    ctrl->bar(140, 170, 320, 230);

    ctrl->setfillstyle(SOLID_FILL, 0x0);
    ctrl->bar(170, 100, 190, 160);

    //Second block
    ctrl->setlinestyle(NULL_LINE, 0, 0);
    ctrl->setbgcolor(0x0044AA);
    ctrl->setfillstyle(LTBKSLASH_FILL, 0xFF0000);
    ctrl->fillellipse(200, 280, 40, 40);

    ctrl->setbgcolor(0x0044AA);
    ctrl->setfillstyle(LTSLASH_FILL, 0xFF0000);
    ctrl->fillellipse(420, 240, 80, 80);

    ctrl->setbgcolor(0x0077FF);
    ctrl->setfillstyle(EMPTY_FILL, 0);
    ctrl->bar(330, 30, 490, 190);

    ctrl->setbgcolor(0x550044AA);
    ctrl->setfillstyle(LTSLASH_FILL, 0x55FF0000);
    ctrl->fillellipse(420, 240, 80, 80);

    //Window
    ctrl->setbgcolor(0xAAFFFF);
    ctrl->setfillstyle(HATCH_FILL, 0);
    ctrl->bar(380, 50, 440, 110);

    ctrl->setlinestyle(SOLID_LINE, 0, 5);
    ctrl->setcolor(0x7FFF0000);
    ctrl->rectangle(382, 52, 438, 108);

    //Back
    ctrl->setfillstyle(SOLID_FILL, 0xAAAAAA);
    ctrl->bar(490, 60, 510, 160);

    ctrl->setlinestyle(DASHED_LINE, 0, 5);
    ctrl->setcolor(0xFF0000);
    ctrl->rectangle(492, 62, 508, 158);

    ctrl->repaint();
    wman->releaseCurrentWindowLock();
    return afterTestSimple(_curIndex);
}

IMG_TEST_F(DX9Int1Test, Star) {
    IController *ctrl = wman->getCurrentWindowAndLock().controller;
    drawGrid(ctrl);
    ctrl->setlinestyle(DASHED_LINE, 0, 4);
    ctrl->moveto(200,200);
    ctrl->linerel(20, 50);

    ctrl->lineto(170, 220);
    ctrl->linerel(60, 0);
    ctrl->linerel(-50, 30);
    ctrl->linerel(20, -50);
    ctrl->repaint();
    wman->releaseCurrentWindowLock();
    return afterTestSimple(_curIndex);
}

IMG_TEST_F(DX9Int1Test, ManyBars) {
    IController *ctrl = wman->getCurrentWindowAndLock().controller;
    ctrl->setfillstyle(SOLID_FILL, 0x7F0000FF);
    for(uint_fast32_t i = 0; i < 50; i++){
        for(uint_fast32_t j = 0; j < HEIGHT; j++){
            ctrl->bar(i,j, i+1, j+1);
        }
    }

    ctrl->repaint();
    wman->releaseCurrentWindowLock();
    return afterTestSimple(_curIndex);
}

IMG_TEST_F(DX9Int1Test, House) {
    IController *ctrl = wman->getCurrentWindowAndLock().controller;
    drawGrid(ctrl);

    //main
    ctrl->setfillstyle(SOLID_FILL, 0x00FF00);
    ctrl->bar(80, 168, 241, 251);

    //windows
    ctrl->setfillstyle(HATCH_FILL, 0x220000);
    ctrl->setcolor(0x220000);
    ctrl->setbgcolor(0x00FFFF);
    ctrl->setlinestyle(SOLID_LINE, 0, 2);
    ctrl->bar(110, 190, 131, 211);
    ctrl->rectangle(110, 190, 130, 210);
    ctrl->bar(190, 190, 211, 211);
    ctrl->rectangle(190, 190, 210, 210);

    //exit
    ctrl->setfillstyle(LINE_FILL, 0x008888);
    ctrl->setcolor(0x220000);
    ctrl->setbgcolor(0x007755);
    ctrl->setlinestyle(NULL_LINE, 0, 0);
    int32_t exitPoints[] = {
            241, 210, 260, 210, 260, 230,
            280, 230, 280, 250, 241, 250
    };
    ctrl->fillpoly(6, exitPoints);

    ctrl->setfillstyle(XHATCH_FILL, 0x7FAAAAAA);
    ctrl->setlinestyle(SOLID_LINE, 0, 1);
    ctrl->setcolor(0);
    ctrl->setbgcolor(0x6C444444);
    ctrl->fillellipse(230, 80, 10, 5);
    ctrl->fillellipse(250, 60, 20, 10);

    //roof
    ctrl->setlinestyle(DASHED_LINE, 0, 5);
    ctrl->setcolor(0x2233EE);
    ctrl->setfillstyle(SLASH_FILL, 0x0000FF);
    ctrl->setbgcolor(0xAAAAAA);
    int32_t roofPoints[] = {
            40, 170, 160, 90, 280, 170
    };
    ctrl->fillpoly(3, roofPoints);

    ctrl->setfillstyle(BKSLASH_FILL, 0x0000FF);
    ctrl->setlinestyle(SOLID_LINE, 0, 2);
    ctrl->bar3d(200, 90, 220, 150, 0, false, true, 0xAAAAFF);

    //Sun
    ctrl->setlinestyle(NULL_LINE, 0, 0);
    ctrl->setfillstyle(SOLID_FILL, 0x00A5FF);
    ctrl->fillellipse(360, 70, 20, 20);
    ctrl->setcolor(0x0095EE);
    ctrl->setlinestyle(SOLID_LINE, 0, 5);

    ctrl->moveto(360, 70);
    ctrl->moverel(-30, 0);
    ctrl->linerel(-20, 0);
    ctrl->moverel(30, -20);
    ctrl->linerel(-20, -20);
    ctrl->moverel(40, 10);
    ctrl->linerel(0, -20);
    ctrl->moverel(20, 30);
    ctrl->linerel(20, -20);
    ctrl->moverel(-10, 40);
    ctrl->linerel(20, 0);
    ctrl->moverel(-30, 20);
    ctrl->linerel(20, 20);
    ctrl->moverel(-40, -10);
    ctrl->linerel(0, 20);
    ctrl->moverel(-20, -30);
    ctrl->linerel(-20, 20);

    ctrl->setlinestyle(SOLID_LINE, 0, 3);
    ctrl->setcolor(0x440000);
    ctrl->setfillstyle(LTSLASH_FILL, 0xFF0000);
    ctrl->setbgcolor(0xFFAA22);
    ctrl->fillellipse(390, 250, 60, 30);

    ctrl->repaint();
    wman->releaseCurrentWindowLock();
    return afterTestSimple(_curIndex);
}
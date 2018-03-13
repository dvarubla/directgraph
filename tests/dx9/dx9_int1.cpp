#include <gtest/gtest.h>
#include "CommonIntegration.h"
#include <testlib/testlib_funcs.h>

class DX9Int1Test: public ImageTest, public CommonIntegration{
public:
    static float WIDTH;
    static float HEIGHT;
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
        p.dpiX = 96.0f;
        p.dpiY = 96.0f;
        _curIndex = wman->createWindow(p);
    }
};

float DX9Int1Test::WIDTH = 561;
float DX9Int1Test::HEIGHT = 401;

IMG_TEST_F(DX9Int1Test, Vehicle){
    IController *ctrl = wman->getCurrentWindowAndLock().controller;
    ctrl->setlinestyle(NULL_LINE, 0, 0);
    ctrl->setfillstyle(SOLID_FILL, 0x994444);
    ctrl->bar(0, 0, WIDTH, HEIGHT);
    ctrl->setfillstyle(SOLID_FILL, 0xFFFFFF);
    for(uint_fast32_t i = 0; i < HEIGHT/10; i++){
        for(uint_fast32_t j = 0; j < WIDTH/10; j++){
            ctrl->bar(j * 10 + 1.0f, i * 10 + 1.0f, (j + 1) * 10.0f, (i + 1) * 10.0f);
        }
    }
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
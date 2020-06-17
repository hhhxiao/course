#include "acllib/acllib.h"
#include "GameFrame.h"
#include<string>
#include <cassert>

const int width = 400;
const int height = 800;
const int fps = 60;
const int planeSpeed = 10;
GameFrame *frame;

//main event cycle
void mainCycle(int timer_id) {
    assert(frame);
    if (timer_id == 0) {
        frame->render();
    }
}

void keyListener(int key, int event) {
    if (key == VK_LEFT) {
        if (frame->getPlaneBox().getX1() > 0)
            frame->planeMove(-planeSpeed, 0);
    }
    if (key == VK_RIGHT) {
        if (frame->getPlaneBox().getX2() < width)
            frame->planeMove(planeSpeed, 0);
    }
    if (key == VK_UP) {
        if (frame->getPlaneBox().getY1() > 0)
            frame->planeMove(0, -planeSpeed);
    }
    if (key == VK_DOWN)
        if (frame->getPlaneBox().getY2() < height)
            frame->planeMove(0, planeSpeed);
    if (key == VK_SPACE && event == KEY_DOWN) {
        frame->toggle();
    }
}

//entry
int Setup() {
    ACL_Image image;
    loadImage("..\\asserts\\hero.jpeg", &image);
    frame = new GameFrame;
    std::string s("plane war");
    initWindow(s.c_str(), DEFAULT, DEFAULT, width, height);
    beginPaint();
    putImage(&image, 160, 400);
    setTextSize(40);
    paintText(40, 100, "Plane War!!!");
    setTextSize(20);
    paintText(55, 600, "press space to start!");
    endPaint();
    registerTimerEvent(mainCycle);
    registerKeyboardEvent(keyListener);
    size_t time_per_frame = 1000 / fps;
    startTimer(0, time_per_frame);

    return 0;
}
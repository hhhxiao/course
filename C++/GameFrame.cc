//
// Created by AgNO3 on 2019/12/24.
//
#include <map>
#include "GameFrame.h"
#include <random>

void GameFrame::init() {
    //加载图片资源
    loadImage("..\\asserts\\hero.jpeg", &plane_res);
    loadImage("..\\asserts\\bullet.jpeg", &bullt_res);
    loadImage("..\\asserts\\enemy.jpeg", &enemy_res);
    //生成飞机
    plane = new HeroSprite(250, 700, &plane_res);
}

void GameFrame::render() {
    update();
    if (!isStart)return;
    beginPaint();
    clearDevice();
    displayScore("score: %d");
    //绘制子弹
    for (auto i:bullets) {
        i->draw();
        i->move(0, 0);
    }
    //绘制敌方方飞机
    for (auto i:enemys) {
        i->draw();
        i->move(0, 0);
    }
    plane->draw();
    endPaint();
}

void GameFrame::memoryFree() {
    //释放即将飞出屏幕的子弹的内存
    for (auto iter = bullets.begin(); iter != bullets.end();) {
        if ((*iter)->getBox().getY1() < 10) {
            auto ptr = *iter;
            iter = bullets.erase(iter);
            delete ptr;
        } else {
            iter++;
        }
    }
}

//随机生成敌军飞机
void GameFrame::geneEnemy() {
    static std::default_random_engine e;
    static std::uniform_int_distribution<int> u(10, 290);
    auto *enemy = new EnemySprite(u(e), 0, &enemy_res);
    enemys.insert(enemy);
}

void GameFrame::geneBullet() {
    auto *bullet = new BulletSprite(plane->getBox().getCenterX(), plane->getBox().getY1() + 20, &bullt_res);
    bullets.insert(bullet);
}

void GameFrame::boxCheck() {
    std::map<EnemySprite *, BulletSprite *> list;
    for (auto i:enemys) {
        if (i->getBox().getY2() > 800 || i->collide(plane)) end();
        for (auto j:bullets) {
            if (i->collide(j)) {
                list.insert({i, j});
            }
        }
    }
    for (auto i:list) {
        score++;
        auto b = i.first;
        auto e = i.second;
        bullets.erase(e);
        enemys.erase(b);
        delete e;
        delete b;
    }
}

//在这里分频来进行各种生成
void GameFrame::update() {
    this->counter++;
    if (this->counter == 60)this->counter = 0;
    if (counter == 0) {
        memoryFree();
    }
    if (counter % 10 == 0) {
        geneBullet();
    }
    if (counter % 20 == 0) {
        if(isStart)
        geneEnemy();
    }
    boxCheck();
}

void GameFrame::end() {
    this->isStart = false;
    beginPaint();
    setTextSize(45);
    paintText(80, 100, "GAME OVER");
    setTextSize(20);
    displayScore("yout final score is %d");
    endPaint();
}

//释放内存
GameFrame::~GameFrame() {
    delete plane;
    for (auto i:enemys) { delete i; }
    for (auto i:bullets) { delete i; }
}


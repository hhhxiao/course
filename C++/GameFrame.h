//
// Created by AgNO3 on 2019/10/10.
//

#ifndef SPRITEGAME_GAMEFRAME_H
#define SPRITEGAME_GAMEFRAME_H

#include "acllib/acllib.h"
#include "sprite/SpriteBase.h"
#include "sprite/HeroSprite.h"
#include "sprite/EnemySprite.h"
#include "sprite/BulletSprite.h"
#include "iostream"
#include <set>
#include <memory>
#include <ctime>
#include <chrono>

class GameFrame {
private:
    //图片资源
    ACL_Image plane_res{}, bullt_res{}, enemy_res{};
    HeroSprite *plane{}; //我方飞机
    std::set<EnemySprite *> enemys; //敌方飞机
    std::set<BulletSprite *> bullets; //子弹
    int counter = 0;  //帧数计数器，用于分频
    bool isStart = false;
    //分数计数器
    int score = 0;
public:
    explicit GameFrame() { init(); } 
    void render(); 
  
    void toggle() { isStart = !isStart; }
    //移动飞机
    void planeMove(int dx, int dy) {plane->move(dx, dy);}
    //获取我方飞机的碰撞箱
    class Rectangle getPlaneBox() {return plane->getBox();}
private:
    //初始化
    void init();

    //渲染
   

    //内存管理(释放子弹的内存)
    void memoryFree();

    //子弹生成
    void geneBullet();

    //敌军飞机生成
    void geneEnemy();

    //碰撞箱检测
    void boxCheck();

    //更新数据
    void update();

    //结束游戏
    void end();



    ~GameFrame();

    void displayScore(const char *format){
        char str[16];
        sprintf(str, format, score);
        paintText(10, 10, str);
    }
   

};


#endif //SPRITEGAME_GAMEFRAME_H

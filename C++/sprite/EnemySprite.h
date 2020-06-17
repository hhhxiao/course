//
// Created by AgNO3 on 2019/12/23.
//

#ifndef SPRITEGAME_ENEMYSPRITE_H
#define SPRITEGAME_ENEMYSPRITE_H


#include "SpriteBase.h"
class  EnemySprite:public SpriteBase {
public:
    void  move(int x,int y)override {
        this->box.move(0,4);
    }
    EnemySprite(int x,int y,ACL_Image*img):SpriteBase(x,y,img){
        box.serRightButtom(x+50,y+35);
    }
};
#endif //SPRITEGAME_ENEMYSPRITE_H

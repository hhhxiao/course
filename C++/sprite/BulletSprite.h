//
// Created by AgNO3 on 2019/12/23.
//


#ifndef SPRITEGAME_BULLETSPRITE_H
#define SPRITEGAME_BULLETSPRITE_H

#include "SpriteBase.h"
class BulletSprite: public SpriteBase  {
public:
    void  move(int, int)override {
        this->box.move(0,-3);
    }
    BulletSprite(int x, int y, ACL_Image *image) : SpriteBase(x, y, image) {
        box.serRightButtom(x+7,y+16);
    }
};
#endif //SPRITEGAME_BULLETSPRITE_H

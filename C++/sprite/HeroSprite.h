//
// Created by AgNO3 on 2019/10/12.
//

#ifndef SPRITEGAME_HEROSPRITE_H
#define SPRITEGAME_HEROSPRITE_H

#include "SpriteBase.h"

class HeroSprite : public SpriteBase {
public:
    HeroSprite(int x, int y, ACL_Image *img) : SpriteBase(x, y, img) {
        box.serRightButtom(x+80,y+100);
    }
    void move(int x, int y) override {
        this->box.move(x,y);
    }
};


#endif //SPRITEGAME_HEROSPRITE_H

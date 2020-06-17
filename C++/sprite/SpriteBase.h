//
// Created by AgNO3 on 2019/10/10.
//

#ifndef SPRITEGAME_SPRITEBASE_H
#define SPRITEGAME_SPRITEBASE_H

#include <iostream>
#include "../acllib/acllib.h"
#include "../Rectangle.h"

//#define DEBUG
class SpriteBase {
protected:
    //x,y是左上角的位置
    class Rectangle box;
    ACL_Image *image;
public:
    virtual void move(int x, int y) = 0;

    SpriteBase(int x, int y, ACL_Image *img) {
        this->box.setLeftTop(x, y);
        this->image = img;
    }

    SpriteBase() : image(nullptr) {}

    struct Rectangle getBox() const {
        return box;
    }

    void draw() {
        putImage(image, box.getX1(), box.getY1());
#ifdef DEBUG
        setBrushColor(EMPTY);
        setPenColor(BLUE);
        box.draw();
#endif
    }
    bool collide(SpriteBase *base) {
        return this->box.collide(base->box);
    }
};
#endif //SPRITEGAME_SPRITEBASE_H

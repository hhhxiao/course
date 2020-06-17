//
// Created by AgNO3 on 2019/12/24.
//

#ifndef SPRITEGAME_RECTANGLE_H
#define SPRITEGAME_RECTANGLE_H

class Rectangle {
    /*
     * X1 y1*********
     *          *
     *          *
     ************ x2,y2
     */
private:
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;


    bool inThis(int x,int y) const{
        return x1<x && x<x2 && y1<y && y<y2;
    }
public:
    Rectangle(int _x1, int _y1, int _x2, int _y2)
            : x1(_x1),
              y1(_y1), x2(_x2), y2(_y2) {}

    Rectangle(int x, int y) : Rectangle(x, y, x, y) {}

    Rectangle() = default;

    int getX1() const { return x1; }

    int getX2() const { return x2; }

    int getY1() const { return y1; }

    int getY2() const { return y2; }

    int getHight() const { return y2 - y1; }

    int getWidth() const { return x2 - x1; }

    int getCenterX() const { return (x2 + x1) / 2; }

    int getCenterY() const { return (y2 + y1) / 2; }

    void setLeftTop(int x, int y) {
        x1 = x;
        y1 = y;
    }

    void serRightButtom(int x, int y) {
        x2 = x;
        y2 = y;
    }

    void  move(int dx,int dy){
        this->x1 += dx;
        this->x2 += dx;
        this->y1 += dy;
        this->y2 += dy;
    }
    void  draw(){
        rectangle(x1,y1,x2,y2);
    }
    bool collide(const Rectangle &r) const {
        return inThis(r.x1,r.y1) || inThis(r.x1,r.y2) ||
        inThis(r.x2,r.y1) || inThis(r.x2,r.y2);
    }
};

#endif //SPRITEGAME_RECTANGLE_H

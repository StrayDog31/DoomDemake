//
// Created by Yakovlev Sergey on 18.05.2024.
//

#ifndef DOOM_SPRITE_H
#define DOOM_SPRITE_H

class Sprite {


public:

    double x;
    double y;
    char tile;

    Sprite(double x, double y, char tile) {

        this->x = x;
        this->y = y;
        this->tile = tile;
    }

    Sprite(Sprite const &sprite) {

        this->x = sprite.x;
        this->y = sprite.y;
        this->tile = sprite.tile;
    }


};


#endif //DOOM_SPRITE_H

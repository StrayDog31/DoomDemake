#ifndef DOOM_LEVEL_H
#define DOOM_LEVEL_H

#include <string>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Font.hpp>
#include <unordered_map>
#include "Sprite.h"

enum class WallTexture {
    Banner,
    Red,
    Pink,
    Grey,
    Blue,
    Gold,
    Wood,
    Rock,

};

enum class SpriteTexture {

    Barrel,
    Pillar,
    Armour,
    Wine,
    Table,
    Kitchen,
    Shrub,
    Furnace,

};


class Level {

public:

    char* map;
    sf::Texture texture;
    sf::RenderStates state;
    sf::Font font;
    int width;
    int height;

    std::vector <Sprite> sprite;

    Level() {

        this->height = 2;
        this->width = 2;
        this->map = new char[height*width];


    }

    void levelOpener(const char* name);
    int levelPrepare();
    bool mapCheck();
    char getTile(int x, int y);

    bool canMove(sf::Vector2f position, sf::Vector2f size);
};

static const std::unordered_map<char, WallTexture> wallTiles {
        {'#', WallTexture::Banner},
        {'=', WallTexture::Red},
        {'|', WallTexture::Pink},
        {'-', WallTexture::Grey},
        {'~', WallTexture::Blue},
        {'@', WallTexture::Gold},
        {'^', WallTexture::Wood},
        {'.', WallTexture::Rock},

};

static const std::unordered_map<char, SpriteTexture> spriteTiles {

        {'B', SpriteTexture::Barrel},
        {'P', SpriteTexture::Pillar},
        {'A', SpriteTexture::Armour},
        {'W', SpriteTexture::Wine},
        {'T', SpriteTexture::Table},
        {'K', SpriteTexture::Kitchen},
        {'S', SpriteTexture::Shrub},
        {'F', SpriteTexture::Furnace}
};

#endif //DOOM_LEVEL_H

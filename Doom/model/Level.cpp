#include <cstring>
#include <fstream>
#include <iostream>
#include "Level.h"

void Level::levelOpener(const char* name) {

    std::fstream source(name, std::ios::in);
    source.seekg(0, std::ios_base::beg);
    char *line = new char[10];
    char *p;

    source.getline(line, 10);
    p = strtok(line, " ");
    delete[] line;

    height = std::atoi(p);
    p = strtok(NULL, " ");
    width = std::atoi(p);

    char *field = new char[height*width];
    int l = 0;
    while (source >> field[l]) l++;
    source.close();

    char *map1 = new char[height*width];

    for(int i = 0; i < height*width; i++) {
        const auto found = spriteTiles.find(field[i]);
        if(found != nullptr) {
            double y = i / width + 0.5f;
            double x = i % width + 0.5f;
            Sprite s(x, y, field[i]);
            field[i] = '.';
            sprite.push_back(s);

        }
        map1[i] = field[i];
    }
    delete[] field;
    map = map1;

}

char Level::getTile(int x, int y) {
    return map[y * width + x];
}

bool Level::mapCheck() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char tile = getTile(x, y);
            // check if tile type is valid
            if (tile != '.' && wallTiles.find(tile) == wallTiles.end()) {
                fprintf(stderr, "Некорректный тайл в [%3d,%3d] (%c)\n", x, y, tile);
                return false;
            }
            // check if edges are walls
            if ((y == 0 || x == 0 || y == height - 1 || x == width - 1) &&
                tile == '.') {
                fprintf(stderr, "Отсутсвует стена в [%3d,%3d]\n", x, y);
                return false;
            }
        }
    }

    state = sf::RenderStates (&texture);

    return true;
}

int Level::levelPrepare() {

    if (!mapCheck()) {
        fprintf(stderr, "Карта некорректна!\n");
        return EXIT_FAILURE;
    }

    if (!font.loadFromFile("../data/font/opensans.ttf")) {
        fprintf(stderr, "Невозможно подгрузить фонт!\n");
        return EXIT_FAILURE;
    }

    if (!texture.loadFromFile("../data/texture/walls.png")) {
        fprintf(stderr, "Невозможно подгрузить текстуру!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool Level::canMove(sf::Vector2f position, sf::Vector2f size) {
    sf::Vector2i upper_left(position - size / 2.0f);
    sf::Vector2i lower_right(position + size / 2.0f);
    if (upper_left.x < 0 || upper_left.y < 0 || lower_right.x >= width || lower_right.y >= height) {
        return false;
    }
    for (int y = upper_left.y; y <= lower_right.y; ++y) {
        for (int x = upper_left.x; x <= lower_right.x; ++x) {
            if (getTile(x, y) != '.') {
                return false;
            }
        }
    }
    return true;
}
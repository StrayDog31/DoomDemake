#ifndef DOOM_FRAME_H
#define DOOM_FRAME_H

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <cstdint>
#include "../model/Level.h"
#include "Level.h"
#include "../controller/Player.h"

class Frame {

public:

    Level level;
    Player player;

    explicit Frame(Level level, Player player) {

        this->lines = sf::VertexArray (sf::Lines, 18 * 1280);
        this->lines.resize(0);

        this->level = level;
        this->player = player;
    };

    ~Frame() {
        lines.clear();
    };

    sf::VertexArray lines;

    void mapDrawer();
    void spriteDrawer();

    const int texture_full_size = 512;
    const int texture_tile_size = 128;

    void frameDrawer();
};


#endif //DOOM_FRAME_H

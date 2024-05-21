#ifndef DOOM_PLAYER_H
#define DOOM_PLAYER_H


#include <SFML/System/Vector2.hpp>
#include "../model/Level.h"

class Player {

public:

    sf::Vector2f position; // Положения игрока в мире
    sf::Vector2f direction; // Направление взгляда
    sf::Vector2f plane; // Рейкаст

    float size_f; // Грань коллизии игрорка
    float moveSpeed; // Скорость
    float rotateSpeed; // Скорость поворота
    sf::Vector2f collision; // Бокс коллизии

    Player() {

        this->position = sf::Vector2f(3.0f, 3.0f);
        this->direction = sf::Vector2f(0.0f, 1.0f);
        this->plane = sf::Vector2f(-0.66f, 0.0f);

        this->size_f = 0.375f;
        this->collision = sf::Vector2f(size_f, size_f);
        this->moveSpeed = 5.0f;
        this->rotateSpeed = 3.0f;
    }



    void Move(bool hasFocus, float dt, Level level);
};


#endif //DOOM_PLAYER_H

#include <SFML/Window/Keyboard.hpp>
#include <valarray>
#include "Player.h"

sf::Vector2f rotateVec(sf::Vector2f vec, float value) {
    return sf::Vector2f(
            vec.x * std::cos(value) - vec.y * std::sin(value),
            vec.x * std::sin(value) + vec.y * std::cos(value)
    );
}

void Player::Move(bool hasFocus, float dt, Level level) {

    if (hasFocus) {
        using kb = sf::Keyboard;

        float moveForward = 0.0f;

        // Считывание клавиш ходьбы
        if (kb::isKeyPressed(kb::W)) {
            moveForward = 1.0f;
        } else if (kb::isKeyPressed(kb::S)) {
            moveForward = -1.0f;
        }

        if (moveForward != 0.0f) {
            sf::Vector2f moveVec = direction * moveSpeed * moveForward * dt;

            if (level.canMove(sf::Vector2f(position.x + moveVec.x, position.y), collision)) {
                position.x += moveVec.x;
            }
            if (level.Level::canMove(sf::Vector2f(position.x, position.y + moveVec.y), collision)) {
                position.y += moveVec.y;
            }
        }
        // Куда поворачиваем
        float rotateDirection = 0.0f;

        // Считывание клавиш поворота
        if (kb::isKeyPressed(kb::A)) {
            rotateDirection = -1.0f;
        } else if (kb::isKeyPressed(kb::D)) {
            rotateDirection = 1.0f;
        }

        // Поворот
        if (rotateDirection != 0.0f) {
            float rotation = rotateSpeed * rotateDirection * dt;
            direction = rotateVec(direction, rotation);
            plane = rotateVec(plane, rotation);
        }
    }
}
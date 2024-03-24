#include <iostream>
#include "LevelLoader.h"
#include <SFML/Graphics.hpp>


int main() {
    const char* source_file = const_cast<char*>("level0.txt");
    LevelLoader::levelOpener(source_file);

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}

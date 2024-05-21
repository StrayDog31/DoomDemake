#include <math.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "../model/Level.h"
#include "../controller/Player.h"
#include "../model/Frame.h"


const static int screenWidth = 1280;
const int screenHeight = 720;

int main() {

    // Создание окна
    sf::RenderWindow window(sf::VideoMode(screenWidth + 1, screenHeight), "Doom Demake");
    window.setSize(sf::Vector2u(screenWidth, screenHeight));
    window.setFramerateLimit(60);

    sf::Clock clock;
    bool hasFocus = true;

    Level level;
    level.levelOpener("../data/levels/level0.txt");
    level.levelPrepare();
    Player player;

    int bal = 0;
    auto start = std::chrono::steady_clock::now();
    for(; bal <= 10;)
    {
        auto current_time = std::chrono::steady_clock::now();
        if(current_time - start >= std::chrono::seconds(1))
        {

            if(window.isOpen()) {

                Frame frame(level, player);

                float dt = clock.restart().asSeconds();

                // SFML events
                sf::Event event;
                while (window.pollEvent(event)) {
                    switch(event.type) {
                        case sf::Event::Closed:
                            window.close();
                            break;
                        case sf::Event::LostFocus:
                            hasFocus = false;
                            break;
                        case sf::Event::GainedFocus:
                            hasFocus = true;
                            break;
                        default:
                            break;
                    }
                }

                frame.frameDrawer();
                player.Move(hasFocus, dt, level);
                window.clear();
                window.draw(frame.lines, level.state);
                window.display();

            }
            else {
                return EXIT_SUCCESS;
            }
        }
    }
}
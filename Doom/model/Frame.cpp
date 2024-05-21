#include <cmath>
#include <valarray>
#include "../controller/Player.h"
#include "Frame.h"


const static int screenWidth = 1280;
const int screenHeight = 720;

double  perpBuffer[screenWidth];

void sortSprites(int* order, double* dist, int amount)
{
    std::vector<std::pair<double, int>> sprites(amount);
    for(int i = 0; i < amount; i++) {
        sprites[i].first = dist[i];
        sprites[i].second = order[i];
    }
    std::sort(sprites.begin(), sprites.end());

    for(int i = 0; i < amount; i++) {
        dist[i] = sprites[amount - i - 1].first;
        order[i] = sprites[amount - i - 1].second;
    }
}

void Frame::spriteDrawer() {

    int spriteOrder[level.sprite.size()];
    double spriteDistance[level.sprite.size()];

    for(int i = 0; i < level.sprite.size(); i++)
    {
        spriteOrder[i] = i;
        spriteDistance[i] = ((player.position.x - level.sprite[i].x) * (player.position.x - level.sprite[i].x) + (player.position.y - level.sprite[i].y) * (player.position.y - level.sprite[i].y)); //sqrt not taken, unneeded
    }
    sortSprites(spriteOrder, spriteDistance, level.sprite.size());

    for(int i = 0; i < level.sprite.size(); i++) {

        // красивый поворот к игроку + положение относительно пола
        double uDiv = 1;
        double vDiv = 1;
        double vMove = 128.0f;

        double spriteX = level.sprite[spriteOrder[i]].x - player.position.x;
        double spriteY = level.sprite[spriteOrder[i]].y - player.position.y;

        double invDet = 1.0 / (player.plane.x * player.direction.y - player.direction.x * player.plane.y); //required for correct matrix multiplication

        double transformX = invDet * (player.direction.y * spriteX  - player.direction.x * spriteY);
        double transformY = invDet * (-player.plane.y * spriteX + player.plane.x * spriteY );

        int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

        int vMoveScreen = int(vMove / transformY);

        // высота спрайта
        int spriteHeight = abs(int(screenHeight / (transformY))) / vDiv;

        int drawStartY = -spriteHeight / 2 + screenHeight / 2 + vMoveScreen;
        if(drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + screenHeight / 2 + vMoveScreen;
        if(drawEndY >= screenHeight) drawEndY = screenHeight - 1;

        // ширина спрайта
        int spriteWidth = abs(int (screenHeight / (transformY))) / uDiv;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if(drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if(drawEndX > screenWidth) drawEndX = screenWidth;

        sf::Color color = sf::Color::White;

        int textureNumber = (int) spriteTiles.find(level.sprite[spriteOrder[i]].tile)->second + wallTiles.size();
        sf::Vector2i textureCoords(
                textureNumber * texture_tile_size % texture_full_size,
                textureNumber * texture_tile_size / texture_full_size * texture_tile_size
        );

        // отрисовка спрайтов
        for(int x = drawStartX; x < drawEndX; x++)
        {
            int texX = abs(int(512 * (x - (spriteWidth / 2 + spriteScreenX)) * texture_tile_size / spriteWidth) / 512);

            if(transformY > 0 && x >= 0 && x <= screenWidth && transformY < perpBuffer[x])
            {
                for(int y = drawStartY; y < drawEndY; y++)
                {
                    int d = (y - vMoveScreen) * 512 - screenHeight * 256 + spriteHeight * 256;
                    int texY = ((d * texture_tile_size) / spriteHeight) / 512;

                    lines.append(sf::Vertex(
                            sf::Vector2f((float) x, (float) y),
                            color,
                            sf::Vector2f((float) textureCoords.x + texX + 8, (float) textureCoords.y + texY)
                    ));
                    lines.append(sf::Vertex(
                            sf::Vector2f((float) drawEndX, (float) drawEndY),
                            color,
                            sf::Vector2f((float) textureCoords.x - 8, (float) textureCoords.y + texY)
                    ));
                }
            }
        }
    }
}

void Frame::mapDrawer() {

    for (int x = 0; x < screenWidth; ++x) {

        float cameraX = 2 * x / (float) screenWidth - 1.0f;
        sf::Vector2f rayPosition = player.position;
        sf::Vector2f rayDirection = player.direction + player.plane * cameraX;

        // расстояние между лучами
        sf::Vector2f deltaDist(
                std::sqrt(1.0f + (rayDirection.y * rayDirection.y) / (rayDirection.x * rayDirection.x)),
                std::sqrt(1.0f + (rayDirection.x * rayDirection.x) / (rayDirection.y * rayDirection.y))
        );

        sf::Vector2i mapPosition(rayPosition); // which box of the map we're in

        sf::Vector2i step;
        sf::Vector2f sideDist;

        // calculate step and initial sideDist
        if (rayDirection.x < 0.0f) {
            step.x = -1;
            sideDist.x = (rayPosition.x - mapPosition.x) * deltaDist.x;
        } else {
            step.x = 1;
            sideDist.x = (mapPosition.x + 1.0f - rayPosition.x) * deltaDist.x;
        }
        if (rayDirection.y < 0.0f) {
            step.y = -1;
            sideDist.y = (rayPosition.y - mapPosition.y) * deltaDist.y;
        } else {
            step.y = 1;
            sideDist.y = (mapPosition.y + 1.0f - rayPosition.y) * deltaDist.y;
        }

        char tile = '.'; // встреченный рейкастом тайл
        bool xParallel; // какой оси координат параллельна стена

        float perpWallDist = 0.0f; // расстояние до стены


        int wallHeight; // Высота стены
        int ceiling = 0; // начальная позиция потолка
        int ground = screenWidth; // начальная позиция пола

        while (tile == '.') {
            if (sideDist.x < sideDist.y) {
                sideDist.x += deltaDist.x;
                mapPosition.x += step.x;
                xParallel = true;
                perpWallDist = (mapPosition.x - rayPosition.x + (1 - step.x) / 2) / rayDirection.x;
            } else {
                sideDist.y += deltaDist.y;
                mapPosition.y += step.y;
                xParallel = false;
                perpWallDist = (mapPosition.y - rayPosition.y + (1 - step.y) / 2) / rayDirection.y;
            }

            wallHeight = screenWidth / perpWallDist;

            // цвет пола
            sf::Color color_f = sf::Color::White;
            color_f.r /= 2;
            color_f.g /= 2;
            color_f.b /= 2;

            // Отрисовка пола
            lines.append(sf::Vertex(
                    sf::Vector2f((float) x, (float) ground),
                    color_f,
                    sf::Vector2f(1.0f, 1.0f)));

            ground = int(wallHeight * 0.4f + screenHeight * 0.5f);

            lines.append(sf::Vertex(
                    sf::Vector2f((float) x, (float) ground),
                    color_f,
                    sf::Vector2f(1.0f, 1.0f)));

            // цвет потолка
            sf::Color color_c = sf::Color::White;
            color_c.r /= 3;
            color_c.g /= 3;
            color_c.b /= 3;

            // Отрисовка потолка
            lines.append(
                    sf::Vertex(sf::Vector2f((float) x, (float) ceiling),
                               color_c,
                               sf::Vector2f(385.0f, 129.0f)));

            ceiling = int(-wallHeight * (1.0f - 0.4f) + screenHeight * 0.5f);

            lines.append(
                    sf::Vertex(sf::Vector2f((float) x, (float) ceiling),
                               color_c,
                               sf::Vector2f(385.0f, 129.0f)));

            tile = level.getTile(mapPosition.x, mapPosition.y);
        }

        // Начальный пиксель нужного куска текстуры на общем полотне
        int textureNumber = (int) wallTiles.find(tile)->second;
        sf::Vector2i textureCoords(
                textureNumber * texture_tile_size % texture_full_size,
                textureNumber * texture_tile_size / texture_full_size * texture_tile_size
        );

        // Точки начала и конца отрисовки стены
        int drawStart = ceiling;
        int drawEnd = ground;

        float wallX;
        if (xParallel) {
            wallX = rayPosition.y + perpWallDist * rayDirection.y;
        } else {
            wallX = rayPosition.x + perpWallDist * rayDirection.x;
        }
        wallX -= floor(wallX);

        // икс координата нужной текстуры на общем полотне
        int texX = int(wallX * float(texture_tile_size));

        // отражение текстуры по горизонтали
        if ((xParallel && rayDirection.x <= 0) || (!xParallel && rayDirection.y >= 0)) {
            texX = texture_tile_size - texX - 1;
        }
        textureCoords.x += texX;

        // цвет стен
        sf::Color color_w = sf::Color::White;

        // иллюзия затемнения стен
        if (xParallel) {
            color_w.r /= 3;
            color_w.g /= 3;
            color_w.b /= 3;
        }

        // Отрисовка стен
        lines.append(sf::Vertex(
                sf::Vector2f((float) x, (float) drawStart),
                color_w,
                sf::Vector2f((float) textureCoords.x, (float) textureCoords.y + 1)
        ));
        lines.append(sf::Vertex(
                sf::Vector2f((float) x, (float) drawEnd),
                color_w,
                sf::Vector2f((float) textureCoords.x, (float) (textureCoords.y + texture_tile_size - 1))
        ));

        perpBuffer[x] = perpWallDist;
    }
}

void Frame::frameDrawer() {

    mapDrawer();
    spriteDrawer();
}
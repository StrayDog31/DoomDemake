#include "LevelLoader.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

void LevelLoader::levelOpener(const char* file_name) {

    std::fstream source(file_name, std::ios::in);
    source.seekg(0, std::ios_base::beg);

    if (!source) {
        std::cout << "Ошибка открытия уровня" << std::endl;
    }

    int length, width;

    source >> length >> width;

    int n = 1, k = 1;
    while ((length/(10*n)) > 1) {n++;}
    while ((width/(10*k)) > 1) {k++;}

    source.seekg(k+n+1, std::ios_base::beg);

    char* map = new char[length*width];

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            char tile;
            source >> tile;
            map[j + i*width] = tile;
        }
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            char tile;
            source >> tile;
            std::cout << map[j + i*width];
        }
        std::cout << std::endl;
    }
}


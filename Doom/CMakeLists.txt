cmake_minimum_required(VERSION 3.26)
project(Doom)

set(CMAKE_CXX_STANDARD 17)

add_executable(Doom main.cpp
        LevelLoader.cpp
        LevelLoader.h)

include_directories (headers/usr/include c:/SFML/include)

set(CMAKE_PREFIX_PATH C:/SFML)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 COMPONENTS network audio graphics window system)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-network sfml-window sfml-system)
target_include_directories(${PROJECT_NAME} PRIVATE src)




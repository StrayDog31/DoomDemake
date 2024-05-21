cmake_minimum_required(VERSION 3.26)

set(project "Doom")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        Frame.cpp
        Level.cpp)

set(${project}_HEADERS
        Frame.h
        Level.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})
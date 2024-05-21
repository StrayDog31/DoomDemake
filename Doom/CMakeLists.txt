cmake_minimum_required(VERSION 3.26)
project(Doom)

set(CMAKE_CXX_STANDARD 17)

include(CheckCXXCompilerFlag)
function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-std=c++11")
enable_cxx_compiler_flag_if_supported("-O3")

add_subdirectory(model)
add_subdirectory(controller)

add_executable(Doom view/main.cpp
            model/Frame.cpp
            model/Frame.h
            model/Level.cpp
            model/Level.h
            controller/Player.cpp
            controller/Player.h
        model/Sprite.cpp
        model/Sprite.h
)

include_directories (headers/usr/include c:/SFML/include)

set(CMAKE_PREFIX_PATH C:/SFML)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 COMPONENTS network audio graphics window system)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-network sfml-window sfml-system)



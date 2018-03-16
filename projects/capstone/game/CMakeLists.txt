cmake_minimum_required(VERSION 3.0)

project(AsteroidsReinformentLearning CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)

add_executable(AsteroidsReinformentLearning asteroid.cpp
    explosion.cpp
    framework.cpp
    game.cpp
    player.cpp
    humanplayer.cpp
    randomplayer.cpp
    renderer.cpp
    sdlrenderer.cpp
    shot.cpp
    spiel.cpp
    sprite.cpp
    texte.cpp
)

target_include_directories(AsteroidsReinformentLearning
    PRIVATE
    ${SDL_INCLUDE_DIR}
    ${SDL_TTF_INCLUE_DIRS}
    ${SDL_IMAGE_INCLUDE_DIRS}
)
target_link_libraries(AsteroidsReinformentLearning
    ${SDL_LIBRARY}
    ${SDL_TTF_LIBRARIES}
    ${SDL_IMAGE_LIBRARIES}
)

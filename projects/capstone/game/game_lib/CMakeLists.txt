cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_subdirectory(SQLiteCpp)

find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)
find_package(sqlite3 REQUIRED)

add_library(GameLib asteroid.cpp
    action.cpp
    databaseconnection.cpp
    explosion.cpp
    framework.cpp
    game.cpp
    player.cpp
    humanplayer.cpp
    randomplayer.cpp
    renderer.cpp
    reinforcementlearningplayer.cpp
    sdlrenderer.cpp
    shot.cpp
    sprite.cpp
    state.cpp
    states.cpp
    texte.cpp
)

target_include_directories(GameLib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
    ${SDL_INCLUDE_DIR}
    ${SDL_TTF_INCLUE_DIRS}
    ${SDL_IMAGE_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/SQListeCpp/include
)
target_link_libraries(GameLib
    ${SDL_LIBRARY}
    ${SDL_TTF_LIBRARIES}
    ${SDL_IMAGE_LIBRARIES}
    SQLiteCpp
    ${SQLITE3_LIBRARIES}
)


cmake_minimum_required(VERSION 3.0)

add_executable(AsteroidsGUI spiel.cpp)

target_link_libraries(
    AsteroidsGUI
    GameLib
)


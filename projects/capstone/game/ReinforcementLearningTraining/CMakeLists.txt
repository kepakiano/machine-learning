cmake_minimum_required(VERSION 3.0)

add_executable(ReinforcementLearningTraining training.cpp)

target_link_libraries(
    ReinforcementLearningTraining
    GameLib
)


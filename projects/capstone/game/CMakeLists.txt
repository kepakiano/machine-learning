project(AsteroidsReinforcementLearning)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build the necessary libraries
add_subdirectory(game_lib)
add_subdirectory(GUI)
add_subdirectory(ReinforcementLearningTraining)
add_subdirectory(ReinforcementLearningTesting)

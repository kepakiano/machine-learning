cmake_minimum_required(VERSION 3.0)

FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
add_executable(ReinforcementLearningTraining training.cpp)

target_link_libraries(
    ReinforcementLearningTraining
    GameLib
    ${Boost_LIBRARIES}
)


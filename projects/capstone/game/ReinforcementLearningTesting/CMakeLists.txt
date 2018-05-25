cmake_minimum_required(VERSION 3.0)

FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
add_executable(ReinforcementLearningTesting testing.cpp)

target_link_libraries(
    ReinforcementLearningTesting
    GameLib
    ${Boost_LIBRARIES}
)


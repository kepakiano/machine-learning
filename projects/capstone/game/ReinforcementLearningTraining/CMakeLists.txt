cmake_minimum_required(VERSION 3.0)

FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
add_executable(ReinforcementLearningTraining training.cpp)

target_link_libraries(
    ReinforcementLearningTraining
    GameLib
    ${Boost_LIBRARIES}
)


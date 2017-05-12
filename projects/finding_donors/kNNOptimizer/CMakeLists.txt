cmake_minimum_required(VERSION 2.8)

project(kNNOptimizer)

file(GLOB SOURCES *.cpp *.h)
aux_source_directory(. SRC_LIST)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic")

if(${DEBUG})
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DDEBUG -g -Og")
else()
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O4 -fopenmp")
endif()

INCLUDE_DIRECTORIES(
  ../../../../../../utilities
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

cmake_minimum_required(VERSION 3.21.2)

project(filter)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
)

file(
  GLOB SOURCES
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_executable(${PROJECT_NAME} ${SOURCES})

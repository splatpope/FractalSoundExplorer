cmake_minimum_required(VERSION 3.10)

project(FSE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB src_files "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.hpp")
set(SOURCES ${src_files})

find_package(OpenGL REQUIRED)
find_package(SFML REQUIRED system window graphics)
set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics)
message("${SFML_LIBRARIES}")

include_directories(${SFML_INCLUDE_DIR})

add_executable(FSE ${SOURCES})
target_link_libraries(FSE ${OPENGL_LIBRARIES} ${SFML_LIBRARIES})

#target_include_directories(FSE PUBLIC ) 
cmake_minimum_required(VERSION 3.10)

project(FSE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE "LEGACY")

set(CMAKE_BINARY_DIR "build")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package(PkgConfig)
pkg_check_modules(LIBCONFIG_PLUSPLUS libconfig++ REQUIRED)
message("${LIBCONFIG_PLUSPLUS_LIBRARIES}")

find_package(OpenGL REQUIRED)
find_package(SFML REQUIRED system window graphics)
set(SFML_LIBRARIES sfml-system sfml-window sfml-graphics)
message("${SFML_LIBRARIES}")

include_directories(${SFML_INCLUDE_DIR})

add_library(Renderer "${SRC_DIR}/Renderer.cpp")
target_include_directories(Renderer PRIVATE "${INCLUDE_DIR}")
add_library(App "${SRC_DIR}/App.cpp" "${SRC_DIR}/Fractals.cpp")
target_include_directories(App PRIVATE "${INCLUDE_DIR}")
target_link_libraries(App Renderer)


add_executable(FSE "${SRC_DIR}/Main.cpp")
target_link_libraries(FSE Renderer App ${LIBCONFIG_PLUSPLUS_LIBRARIES} ${OPENGL_LIBRARIES} ${SFML_LIBRARIES})
target_include_directories(FSE PRIVATE "${INCLUDE_DIR}")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
file(COPY ${SRC_DIR}/static/ DESTINATION ${CMAKE_BINARY_DIR}/static/)
file(COPY ${SRC_DIR}/default.cfg DESTINATION ${CMAKE_BINARY_DIR})

#target_include_directories(FSE PUBLIC ) 
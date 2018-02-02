cmake_minimum_required(VERSION 2.6)
project(game)
add_executable(game src/main.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(GLEW REQUIRED)
add_subdirectory(lib/glfw)

target_link_libraries(game ${GLEW_LIBRARIES})
target_link_libraries(game glfw)
find_package(OpenGL REQUIRED)

include_directories(
    ${GLFW_INCLUDE_PATH}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_PATH}
)

target_link_libraries(game
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
)

set(ENV{SYSTEM} linux-egl)
set(ENV{GLFW_USE_WAYLAND} ON)


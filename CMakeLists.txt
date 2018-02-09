cmake_minimum_required(VERSION 2.6)
project(game)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(GLEW REQUIRED)
add_subdirectory(lib/glfw)
set(GLFW_LIBRARY glfw)

find_package(OpenGL REQUIRED)

set(INCLUDE_DIRS
    src
)

set(SOURCE_FILES
    src/main.cpp
    src/shader.cpp
    src/render_group.cpp
    src/render_info.cpp
)

include_directories(
    ${GLFW_INCLUDE_PATH}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_PATH}
    ${INCLUDE_DIRS}
)

add_executable(game
    ${SOURCE_FILES}
)

target_link_libraries(game
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message("Mac OS detected")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message("Linux detected")

    set(ENV{SYSTEM} linux-egl)
    set(ENV{GLFW_USE_WAYLAND} ON)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message("Windows detected")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


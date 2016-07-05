cmake_minimum_required (VERSION 3.5)
project (sunstone-2d)
add_executable(sunstone-2d game.cpp)

add_subdirectory(./libs/glfw)
include_directories(./libs/glfw)
target_link_libraries(sunstone-2d glfw ${GLFW_LIBRARIES})

find_library(COCOA_LIBRARY Cocoa)
find_library(OpenGL_LIBRARY OpenGL)
mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY})

target_link_libraries(sunstone-2d stdc++ "-framework OpenGL" "-framework Cocoa")


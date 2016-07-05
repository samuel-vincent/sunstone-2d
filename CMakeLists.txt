cmake_minimum_required (VERSION 3.5)
project (sunstone-2d)
add_executable(sunstone-2d game.cpp glew.c)

add_subdirectory(./libs/glfw)
include_directories(./libs/glfw ./)
target_link_libraries(sunstone-2d glfw ${GLFW_LIBRARIES})

if (APPLE)
   target_link_libraries(sunstone-2d stdc++ "-framework OpenGL" "-framework Cocoa")
elseif (WIN32)
   find_package(OpenGL REQUIRED)
   target_link_libraries(sunstone-2d ${OPENGL_LIBRARIES})
endif()


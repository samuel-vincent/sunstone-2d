cmake_minimum_required (VERSION 3.5)
project (sunstone-2d)
add_executable(sunstone-2d game.cpp glew.c)

add_subdirectory(./libs/glfw)
add_subdirectory(./libs/glm)
include_directories(./libs/glfw ./libs/glm ./)
target_link_libraries(sunstone-2d glfw ${GLFW_LIBRARIES})

set_property(TARGET sunstone-2d PROPERTY CXX_STANDARD 11)	

if (APPLE)
   target_link_libraries(sunstone-2d stdc++ "-framework OpenGL" "-framework Cocoa")
elseif (WIN32)
   find_package(OpenGL REQUIRED)
   target_link_libraries(sunstone-2d ${OPENGL_LIBRARIES})
endif()


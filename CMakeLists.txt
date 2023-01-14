cmake_minimum_required(VERSION 3.20)
project(BillardGL)

set(CMAKE_CXX_STANDARD 14)

#set the minimal version of cmake and the minimal version of the c++ language
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

#for glad library
add_library( glad STATIC 3rdParty/glad/src/glad.c)
set(GLAD_INCLUDE "3rdParty/glad/include")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE})

#for GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdParty/glfw)


# Assimp
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
configure_file("${PROJECT_SOURCE_DIR}/3rdparty/assimp/include/assimp/config.h.in" "${PROJECT_SOURCE_DIR}/3rdparty/assimp/include/assimp/config.h")
configure_file("${PROJECT_SOURCE_DIR}/3rdparty/assimp/contrib/zlib/zconf.h.cmakein" "3rdparty/assimp/contrib/zlib/zconf.h")

target_link_libraries(glad glfw ${OPENGL_LIBRARIES} assimp)

include_directories(3rdParty/glad/include/
        3rdParty/glfw/include/
        3rdParty/glm/
        3rdParty/assimp/include
        3rdParty/stb/
        models/
        textures/
        src/)



file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(BillardGL ${SOURCE_FILES})

target_link_libraries(BillardGL glad glfw ${OPENGL_LIBRARIES})

add_compile_definitions(PATH_TO_OBJECTS="${CMAKE_CURRENT_SOURCE_DIR}/models")
add_compile_definitions(PATH_TO_TEXTURE="${CMAKE_CURRENT_SOURCE_DIR}/textures")

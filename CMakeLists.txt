cmake_minimum_required(VERSION 3.16)
project(SFML_Learning LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(game 
src/main.cpp
src/Game.cpp 
src/Game.hpp
src/Entity.hpp 
src/Entity.cpp
src/Aircraft.hpp 
src/Aircraft.cpp 
src/SpriteNode.hpp
src/SpriteNode.cpp 
src/World.hpp
src/World.cpp
src/ResourceHolder.hpp
src/ResourceEnums.hpp
src/SceneNode.hpp
src/SceneNode.cpp)

target_link_libraries(game PRIVATE sfml-graphics)
target_compile_features(game PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET game
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:game>
        VERBATIM)
endif()

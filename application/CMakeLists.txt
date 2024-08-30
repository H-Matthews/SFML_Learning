cmake_minimum_required(VERSION 3.16)

set(SRC src/main.cpp src/Game.cpp
        src/Entity.cpp src/Aircraft.cpp
        src/SpriteNode.cpp src/World.cpp
        src/SceneNode.cpp src/CommandQueue.cpp 
        src/PlayerInput.cpp src/Command.cpp)
        
set (INC inc/Game.hpp inc/Entity.hpp 
        inc/Aircraft.hpp inc/SpriteNode.hpp
        inc/SpriteNode.hpp inc/World.hpp 
        inc/ResourceHolder.hpp inc/ResourceEnums.hpp 
        inc/SceneNode.hpp inc/CommandQueue.hpp 
        inc/PlayerInput.hpp inc/Category.hpp 
        inc/Command.hpp)

add_executable(game ${SRC} ${INC})

target_link_libraries(game PRIVATE sfml-graphics utility)
target_include_directories(utility PUBLIC ../lib/utility/inc inc/)
target_compile_features(game PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET game
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:game>
        VERBATIM)
endif()

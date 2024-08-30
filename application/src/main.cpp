#include "Game.hpp"
#include "Category.hpp"

// From Utility Lib
#include "Logger.hpp"

#include <iostream>
#include <stdexcept>

int main()
{

    Utility::Logger::log("This is a message");
    try
    {
        Game game;
        game.run();
    }
    catch (std::exception& e )
    {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
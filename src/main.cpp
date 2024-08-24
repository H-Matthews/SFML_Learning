#include "Game.hpp"
#include "Category.hpp"

#include <iostream>
#include <stdexcept>

int main()
{
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
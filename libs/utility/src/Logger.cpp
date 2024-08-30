#include "Logger.hpp"

#include <iostream>

void Utility::Logger::log(std::string message)
{
    std::cout << message << std::endl;
}
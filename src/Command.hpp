#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <SFML/System/Time.hpp>

class SceneNode;

struct Command
{
    Command();
    std::function<void(SceneNode&, sf::Time)> action; 
    unsigned int category;
};

#endif
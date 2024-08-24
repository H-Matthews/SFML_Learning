#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "CommandQueue.hpp"

class Player
{
    public:
        enum Action
        {
            MoveLeft,
            MoveRight,
            MoveUp,
            MoveDown,
            ActionCount
        };

        Player();
        void handleEvent(const sf::Event& event, CommandQueue& commands);
        void handleRealTimeInput(CommandQueue& commands);

        void assignKey(Action action, sf::Keyboard::Key key);
        sf::Keyboard::Key getAssignedKey(Action action) const;

    private:
        void initializeActions();
        static bool isRealTimeAction(Action action);

    std::map<sf::Keyboard::Key, Action> mKeyBinding;
    std::map<Action, Command> mActionBinding;
};

#endif
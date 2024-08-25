#include "PlayerInput.hpp"
#include "Aircraft.hpp"

#include <iostream>

struct AircraftStatistics
{
    void operator() (Aircraft& aircraft, sf::Time dt) const
    {
        std::cout << "X: " << aircraft.getPosition().x << std::endl;
        std::cout << "Y: " << aircraft.getPosition().y << std::endl;
    }
};

struct AircraftMover
{
    AircraftMover(float vx, float vy) : 
        velocity(vx, vy)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        aircraft.accelerate(velocity);
    }

    sf::Vector2f velocity;
};

PlayerInput::PlayerInput()
{
    // Set Initial Key Bindings
    mKeyBinding[sf::Keyboard::A] = MoveLeft;
    mKeyBinding[sf::Keyboard::D] = MoveRight;
    mKeyBinding[sf::Keyboard::W] = MoveUp;
    mKeyBinding[sf::Keyboard::S] = MoveDown;

    // Set initial Action Bindings
    initializeActions();

    for(auto& pair: mActionBinding)
    {
        pair.second.category = Category::PlayerAircraft;
    }
}

void PlayerInput::handleRealTimeInput(CommandQueue& commands)
{
    for(auto pair: mKeyBinding)
    {
        if(sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
            commands.push(mActionBinding[pair.second]);
    }
}

void PlayerInput::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if(event.type == sf::Event::KeyPressed)
    {
        auto found = mKeyBinding.find(event.key.code);
        if( found != mKeyBinding.end() && !isRealTimeAction(found->second))
            commands.push(mActionBinding[found->second]);
    }
}

void PlayerInput::assignKey(Action action, sf::Keyboard::Key key)
{
    // Remove all keys that already map to an action
    for(auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
    {
        if(itr->second == action)
            mKeyBinding.erase(itr++);
        else 
            ++itr;
    }

    // Insert new binding
    mKeyBinding[key] = action;
}


sf::Keyboard::Key PlayerInput::getAssignedKey(Action action) const
{
    sf::Keyboard::Key keyValue;
    for(auto pair: mKeyBinding)
    {
        if(pair.second == action)
            keyValue = pair.first;
    }

    return keyValue;
}

void PlayerInput::initializeActions()
{
    const float playerSpeed = 150.f;

    mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
    mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
    mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
    mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));
    mActionBinding[DisplayACPosition].action = derivedAction<Aircraft>(AircraftStatistics());

}

bool PlayerInput::isRealTimeAction(Action action)
{
    bool isAction = false;

    switch(action)
    {
        case MoveLeft:
        case MoveRight:
        case MoveUp:
        case MoveDown:
            isAction = true;
            break;
        default:
            isAction = false;
    }

    return isAction;
}
        
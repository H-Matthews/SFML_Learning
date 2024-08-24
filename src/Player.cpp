#include "Player.hpp"
#include "Aircraft.hpp"

#include <iostream>

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

Player::Player()
{
    // Set Initial Key Bindings
    mKeyBinding[sf::Keyboard::Left] = MoveLeft;
    mKeyBinding[sf::Keyboard::Right] = MoveRight;
    mKeyBinding[sf::Keyboard::Up] = MoveUp;
    mKeyBinding[sf::Keyboard::Down] = MoveDown;

    // Set initial Action Bindings
    initializeActions();

    for(auto& pair: mActionBinding)
    {
        pair.second.category = Category::PlayerAircraft;
    }
}

void Player::handleRealTimeInput(CommandQueue& commands)
{
    for(auto pair: mKeyBinding)
    {
        if(sf::Keyboard::isKeyPressed(pair.first) && isRealTimeAction(pair.second))
            commands.push(mActionBinding[pair.second]);
    }
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if(event.type == sf::Event::KeyPressed)
    {
        auto found = mKeyBinding.find(event.key.code);
        if( found != mKeyBinding.end() && !isRealTimeAction(found->second))
            commands.push(mActionBinding[found->second]);
    }
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
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


sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
    sf::Keyboard::Key keyValue;
    for(auto pair: mKeyBinding)
    {
        if(pair.second == action)
            keyValue = pair.first;
    }

    return keyValue;
}

void Player::initializeActions()
{
    const float playerSpeed = 200.f;

    mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
    mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
    mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
    mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));

}

bool Player::isRealTimeAction(Action action)
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
        
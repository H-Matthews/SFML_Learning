#include "Entity.hpp"


void Entity::setVelocity(const sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(const float vx, const float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
    return mVelocity;
}

void Entity::updateCurrent(sf::Time timeStep)
{
    move(mVelocity * timeStep.asSeconds());
}
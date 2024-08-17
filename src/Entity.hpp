#ifndef ENTITY_H
#define ENTITY_H

#include<SFML/Graphics.hpp>

#include "SceneNode.hpp"

class Entity : public SceneNode
{
    public:
        void setVelocity(const sf::Vector2f velocity);
        void setVelocity(const float vx, const float vy);
        sf::Vector2f getVelocity() const;

    private:
        sf::Vector2f mVelocity;
        virtual void updateCurrent(sf::Time timeStep);
};


#endif
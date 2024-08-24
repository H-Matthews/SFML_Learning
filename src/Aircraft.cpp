#include "Aircraft.hpp"

#include <iostream>

Textures::ID toTextureID(Aircraft::Type type)
{
    Textures::ID tempType;
    switch(type)
    {
        case Aircraft::Eagle:
            tempType = Textures::Eagle;
            break;
        case Aircraft::Raptor:
            tempType = Textures::Raptor;
            break;
        default:
            tempType = Textures::Eagle;
    }

    return tempType;
}

Aircraft::Aircraft(Type type, const TextureHolder& textures) : mType(type), 
                mSprite(textures.get(toTextureID(type)))
{
    // This sets the origin to the middle of the sprite
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f );
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

unsigned int Aircraft::getCategory() const
{
    Category::Type type;

    switch (mType)
    {
        case Eagle:
            type = Category::PlayerAircraft;
            break;
        default:
            type = Category::EnemyAircraft;
    }

    std::cout << type << std::endl;

    return type;
}
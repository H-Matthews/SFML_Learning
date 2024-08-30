#ifndef RESOURCE_ENUMS_H
#define RESOURCE_ENUMS_H

// Forward Declaration of SFML Classes
namespace sf
{
    class Texture;
}

namespace Textures
{
    enum ID
    {
        Eagle = 0,
        Raptor,
        Desert
    };
}

// Forward declaration of ResourceHolder Class
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;

#endif
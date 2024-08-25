#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "ResourceHolder.hpp"
#include "ResourceEnums.hpp"
#include "Entity.hpp"

class Aircraft : public Entity
{
    public:

        enum Type
        {
            Eagle = 0, 
            Raptor
        };

        // Define as explicit to prevent compiler from converting type implicitly
        explicit Aircraft(Type type, const TextureHolder& textures);
        
        // Derives from SceneNode
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual uint32_t getCategory() const override;

    private:
        Type mType;
        sf::Sprite mSprite;

};



#endif
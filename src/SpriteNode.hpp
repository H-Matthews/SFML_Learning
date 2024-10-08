#ifndef SPRITE_NODE_H
#define SPRITE_NODE_H

#include "SceneNode.hpp"

class SpriteNode: public SceneNode
{
    public:
        explicit SpriteNode(const sf::Texture& texture);
        SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

    private:
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        sf::Sprite mSprite;
};

#endif
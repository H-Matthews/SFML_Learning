#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "Category.hpp"
#include "Command.hpp"

class SceneNode : public sf::Transformable, public sf::Drawable
{
    public:
        SceneNode();
        void update(sf::Time timeStep);

        // Command Event Messaging Driver
        void onCommand(const Command& command, sf::Time timeStep);

        sf::Transform getWorldTransform() const;
        sf::Vector2f getWorldPosition() const;

        void attachChild(std::unique_ptr<SceneNode> child);
        std::unique_ptr<SceneNode> detachChild(const SceneNode& node);

    private:
        std::vector<std::unique_ptr<SceneNode>> mChildren;
        SceneNode* mParent;

    private:
        virtual void updateCurrent(sf::Time timeStep);
        void updateChildren(sf::Time timeStep);

        // Draw is derived from Drawable and is called from SFMLs Window
        // That is why its listed as private
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

        virtual unsigned int getCategory() const;
        void commandChildren(const Command& command, sf::Time timeStep) const;
};


#endif
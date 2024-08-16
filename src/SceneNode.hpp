#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <SFML/Graphics.hpp>
#include <memory>

class SceneNode : public sf::Transformable, public sf::Drawable
{
    public:
        SceneNode();
        void update(sf::Time dt);
        sf::Transform getWorldTransform() const;
        sf::Vector2f getWorldPosition() const;

        void attachChild(std::unique_ptr<SceneNode> child);
        std::unique_ptr<SceneNode> detachChild(const SceneNode& node);

    private:
        std::vector<std::unique_ptr<SceneNode>> mChildren;
        SceneNode* mParent;

        virtual void updateCurrent(sf::Time dt);
        void updateChildren(sf::Time dt);

        // Derived from Drawable
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

};


#endif
#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <array>

#include "ResourceHolder.hpp"
#include "ResourceEnums.hpp"
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "CommandQueue.hpp"

class World : private sf::NonCopyable
{
    public:
        explicit World(sf::RenderWindow& window);
        void update(sf::Time timeStep);
        void draw();

        CommandQueue& getCommandQueue();

    private:
        enum Layer
        {
            Background = 0,
            Air,
            LayerCount
        };

    private:
        void loadTextures();
        void buildScene();

        void adaptPlayerVelocity();
        void adaptPlayerPosition();

    private:
        sf::RenderWindow& mWindow;
        sf::View mWorldView;
        TextureHolder mTextures;

        SceneNode mSceneGraph;
        std::array<SceneNode*, LayerCount> mSceneLayers;

        sf::FloatRect mWorldBounds;
        sf::Vector2f mSpawnPosition;
        float mScrollSpeed;
        Aircraft* mPlayerAircraft;
        CommandQueue mCommandQueue;
};

#endif
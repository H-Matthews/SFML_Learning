#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "ResourceHolder.hpp"

namespace Textures
{
    enum ID
    {
        Landscape = 0,
        Airplane,
    };
}

class Game
{
    public:
        Game();
        void run();

    private:
        void processEvents();
        void update(sf::Time deltaTime);
        void render();

        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void updateStatistics(sf::Time elapsedTime);
        std::string toString(std::size_t value);

        static const sf::Time TimePerFrame;
        static const float PlayerSpeed;

        sf::Sprite mLandScape;
        ResourceHolder<sf::Texture, Textures::ID> mTextures;
        sf::RenderWindow mWindow;
        sf::Sprite mPlayerPlane;
        sf::Font mFont;
        sf::Text mStatisticsText;
        sf::Time mStatisticsUpdateTime;

        std::size_t mStatisticsNumFrames;
        bool mIsMovingUp;
        bool mIsMovingDown;
        bool mIsMovingLeft;
        bool mIsMovingRight;
};

#endif
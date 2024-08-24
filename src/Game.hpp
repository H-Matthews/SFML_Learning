#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "ResourceHolder.hpp"
#include "World.hpp"
#include "Player.hpp"

class Game
{
    public:
        Game();
        void run();

    private:
        void processInput();
        void update(sf::Time deltaTime);
        void render();

        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void updateStatistics(sf::Time elapsedTime);
        std::string toString(std::size_t value);

        static const sf::Time TimePerFrame;
        static const float PlayerSpeed;

        sf::RenderWindow mWindow;
        World mWorld;
        Player mPlayer;

        sf::Font mFont;
        sf::Text mStatisticsText;
        sf::Time mStatisticsUpdateTime;
        std::size_t mStatisticsNumFrames;
};

#endif
#include "Game.hpp"

#include <string>
#include <iostream>
#include <sstream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 120.f);
const float Game::PlayerSpeed = 50;

Game::Game() :
    mWindow(sf::VideoMode(640, 480), "Game World", sf::Style::Close),
    mFont(),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0),
    mWorld(mWindow)
{
    mFont.loadFromFile("../media/Sansation.ttf");
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(15);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while(mWindow.isOpen())
    {

        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while( timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            update(TimePerFrame);
        }

        updateStatistics(elapsedTime);
        render();
    }

}

void Game::processEvents()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed: {
                handlePlayerInput(event.key.code, true);
                break;
            }
            case sf::Event::KeyReleased: {
                handlePlayerInput(event.key.code, false);
                break;
            }
            case sf::Event::Closed: {
                mWindow.close();
                break;
            }
        }
    }
}

void Game::render()
{
    mWindow.clear();
    mWorld.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::update(sf::Time fixedTimeStep)
{
    std::cout << "Time Per Frame as seconds: " << fixedTimeStep.asSeconds() << std::endl;
    std::cout << "Time Per Frame as milliseconds: " << fixedTimeStep.asMicroseconds() << std::endl;

    mWorld.update(fixedTimeStep);
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
}

std::string Game::toString(std::size_t value)
{
    std::stringstream s;
    s << value; 

    return s.str();
}


// Function calculates how many frames we are cranking per second
// mStatisticsNumFrames represents ours FPS
void Game::updateStatistics(sf::Time elapsedTime)
{
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;

    if( mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString(
            "Frames / Second = " + toString(mStatisticsNumFrames) + "\n" + 
            "Time / Update " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}
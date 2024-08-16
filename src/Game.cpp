#include "Game.hpp"

#include <string>
#include <iostream>
#include <sstream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 120.f);
const float Game::PlayerSpeed = 50;

Game::Game() :
    mTextures(),
    mWindow(sf::VideoMode(640, 480), "SFML Tutorial Application"),
    mPlayerPlane(),
    mLandScape(), 
    mFont(),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0),
    mIsMovingUp(false),
    mIsMovingDown(false),
    mIsMovingLeft(false),
    mIsMovingRight(false)
{
    mTextures.load(Textures::Airplane, "../media/Textures/Airplane.png");
    mTextures.load(Textures::Landscape, "../media/Textures/Desert.png");

    mPlayerPlane.setTexture(mTextures.get(Textures::Airplane));
    mPlayerPlane.setPosition(100.f, 100.f);

    mLandScape.setTexture(mTextures.get(Textures::Landscape));

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
        processEvents();

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
    mWindow.draw(mLandScape);
    mWindow.draw(mPlayerPlane);
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::update(sf::Time fixedTimeStep)
{
    std::cout << "Time Per Frame as seconds: " << fixedTimeStep.asSeconds() << std::endl;
    std::cout << "Time Per Frame as milliseconds: " << fixedTimeStep.asMicroseconds() << std::endl;

    sf::Vector2f movement(0.f, 0.f);

    if(mIsMovingUp)
        movement.y -= 1.f;

    if(mIsMovingDown)
        movement.y += 1.f;
        
    if(mIsMovingLeft)
        movement.x -= 1.f;
       
    if(mIsMovingRight)
        movement.x += 1.f;
    
    // Multiply our movement by a fixed time step every update iteration.
    // We only call update when we have gone above the alloted threshold
    mPlayerPlane.move((movement * fixedTimeStep.asSeconds()) * PlayerSpeed);
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if(key == sf::Keyboard::W)
        mIsMovingUp = isPressed;
    else if(key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
    else if(key == sf::Keyboard::A)
        mIsMovingLeft = isPressed;
    else if(key == sf::Keyboard::D)
        mIsMovingRight = isPressed;
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
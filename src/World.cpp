#include "World.hpp"
#include "SpriteNode.hpp"

#include <iostream>
#include <cmath>

World::World(sf::RenderWindow& window) : 
    mWindow(window),
    mWorldView(window.getDefaultView()),
    mTextures(),
    mSceneGraph(),
    mSceneLayers(),
    mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f),
    mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - (mWorldView.getSize().y / 2.f)),
    mScrollSpeed(-50.f),
    mPlayerAircraft(nullptr)
{
    loadTextures();
    buildScene();

    // Prepare the View
    mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time timeStep)
{
    // Scroll the World
    mWorldView.move(0.f, mScrollSpeed * timeStep.asSeconds());

    // Make sure PlayerAircraft only moves on Player input
    mPlayerAircraft->setVelocity(0.f, 0.f);

    // Forward all commands in queue to Scene Graph
    while(!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), timeStep);

    // Determines whether or not we changed our X and Y in the same update cycle
    // If so, we must correct our velocity values because we will likely be going too fast
    adaptPlayerVelocity();

    // Apply movements; After update apply movement adjustment if necessary
    mSceneGraph.update(timeStep);
    adaptPlayerPosition();
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
    // Paths are relative to the bin directory
    mTextures.load(Textures::Eagle, "../../media/Textures/Eagle.png");
    mTextures.load(Textures::Raptor, "../../media/Textures/Raptor.png");
    mTextures.load(Textures::Desert, "../../media/Textures/Desert.png");
}

void World::buildScene()
{
    // Initialize Layers
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        std::unique_ptr<SceneNode> layer(new SceneNode());
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    // Prepare the tiled background
    sf::Texture& texture = mTextures.get(Textures::Desert);
    sf::IntRect textureRect(mWorldBounds);
    texture.setRepeated(true);

    // Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
	mSceneLayers[Air]->attachChild(std::move(leader));

	// Add two escorting aircrafts, placed relatively to the main plane
	std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(leftEscort));

	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, mTextures));
	rightEscort->setPosition(80.f, 50.f); 
	mPlayerAircraft->attachChild(std::move(rightEscort));
}

void World::adaptPlayerPosition()
{
    // Keep players position inside screen bounds, at least borderDistance units from the border
    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    const float borderDistance = 40.f;

    // Update Position based on boundary
    sf::Vector2f position = mPlayerAircraft->getPosition();
    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);

    mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity()
{
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();

    if(velocity.x != 0.f && velocity.y != 0.f)
    {
        mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
    }
}

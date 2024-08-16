#include "World.hpp"
#include "SpriteNode.hpp"


World::World(sf::RenderWindow& window) : 
    mWindow(window),
    mWorldView(window.getDefaultView()),
    mTextures(),
    mSceneGraph(),
    mSceneLayers(),
    mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f),
    mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f),
    mScrollSpeed(-50.f),
    mPlayerAircraft(nullptr)
{
    loadTextures();
    buildScene();

    // Prepare the View
    mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
    // Scroll the World
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());

    // Move the player sideways (plane scout follow the main aircraft)
    sf::Vector2f position = mPlayerAircraft->getPosition();
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();

    // If the player touches borders, flip its X velocity
    if(position.x <= mWorldBounds.left + 150.f || position.x >= mWorldBounds.left + mWorldBounds.width - 150.f)
    {
        velocity.x = -velocity.x;
        mPlayerAircraft->setVelocity(velocity);
    }

    // Apply movements
    mSceneGraph.update(dt);
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
    mTextures.load(Textures::Eagle, "../media/Textures/Eagle.png");
    mTextures.load(Textures::Raptor, "../media/Textures/Raptor.png");
    mTextures.load(Textures::Desert, "../media/Textures/Desert.png");
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

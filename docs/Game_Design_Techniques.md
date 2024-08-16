# Game Design Techniques
Attempting to develop a 2D game has shown me that I dont know much about how basic games work. This document is intended to explain underlying concepts in game design that are new to me. 

## Content
[Game Loops and Frames](./Game_Design_Techniques.md#game-loops-and-frames)
[Resource Management](./Game_Design_Techniques.md#resource-management)
[Sequential Rendering](./Game_Design_Techniques.md#sequential-rendering)

### Game Loops and Frames

A game loop is defined as the following at a really basic level:

``` c++
void Game::run()
{
    while(gameIsRunning)
    {
        processEvents();
        update();
        render();
    }
}
```

This introduces an issue where our game will behave differently depending on the underlying hardware. Our hardware will attempt to execute this however fast it can. Meaning our game will behave differently depending on our hardware speed.

For example, on a modern PC, the input values I chose for the movement might be just right for how fast the game is running on THIS machine. However, if we go to an older machine the values may not be high enough and our movement will be remarkably slower. 

We need a way to create a uniform experience across all platforms which is why we have to move to a concept called frame independence. The movement issue can be solved by an old formula: *distance = speed * time* 

This will enable us to calculate a relevant speed for every frame, so that our game object travels exactly the same distance over one second.

Using this strategy the update function would look like this

``` c++
    void Game::update(sf::Time deltaTime)
    {
        sf::vector2f movement(0.f, 0.f);
        if (mIsMovingUp)
            movement.y -= PlayerSpeed;
        if (mIsMovingDown)
            movement.y += PlayerSpeed;
        if (mIsMovingLeft)
            movement.x -= PlayerSpeed;
        if (mIsMovingRight)
            movement.x += PlayerSpeed;
    }

    mPlayer.move(movement * deltaTime.asSeconds());

```

Every frame we calculate the distance we want to travel based on the speed and time. 
Our time is the time elapsed since we last called the update function. It is defined in the run function below

``` c++

    void Game::run()
    {
        sf::Clock clock
        while(mWindow.isOpen())
        {
            sf::Time deltaTime = clock.restart();
            processEvent();
            update(deltaTime);
            render();
        }
    }
```
The only difference is that now we are measuring the elapsed time since the last update with sf::Clock and are now storing it in deltaTime which we use to compute our distance traveled. The circle will now appear to move more uniformly instead of moving extremely fast.

#### The Issue with variable time steps
With using a deltaTime, we now have an issue that a frame may sometime take three times as long depending on the game logic and applying that much time to your distance will make the character start teleporting. Using delta time steps can also mess up the physics of your game depending on the implementation. There are a couple of good resources I will link that explains this in much greater detail such as [Game Loops](https://gameprogrammingpatterns.com/game-loop.html)

The solution is to move towards a fixed time step.

#### Fixed Time Steps
In order to get 100% reproducible steps, we move to a fixed time step. Meaning that the time value we use every update will now be fixed. Our main game loop must now gurarantee that in any circumstance, we always give the same time step to the update function. This will introduce an additional while loop into our main game loop

``` c++

    void Game::run()
    {
        sf::Clock clock
        sf::Time timeSinceLastUpdate = sf::Time::Zero;

        while(mWindow.isOpen())
        {
            processEvents();

            timeSinceLastUpdate += clock.restart();

            while(timeSinceLastUpdate > TimePerFrame)
            {
                timeSinceLastUpdate -= TimePerFrame;
                processEvents();
                update(TimePerFrame);
            }

            render();
        }
    }
```

The modified code will now collect the elapsed time since we last called our update function and only call update when we exceed the TimePerFrame value. This ensure that we only call update at fixed time intervals, and if our program gets behind, it will call update many times before calling render. TimePerFrame should be set to 60 frames per second so ``` sf::seconds TimePerFrame(1.5 / 60.f); ``` You could also increment this to 120 if you wanted.

The small downside is that if we calculate to slow, then our game will stutter. Meaning if timeSinceLastUpdate gets too large and we have to do too many updates before a render(). It will cause our game to stutter. This allows for techniques such as interpolations to smoothen game flow.

#### Calculating FPS
In order to calculate how many frames we are rendering per second. We need a helper function that looks like the following.

``` c++

    void Game::updateStatistics(sf::Time elapsedTime)
    {
        mStatisticsUpdateTime += elapsedTime;
        mStatisticsNumFrames += 1;

        // When it has been a second, display the amount of frames accumulated
        if( mStatisticsUpdateTime >= sf::seconds(1.0f))
        {
             mStatisticsText.setString(
            "Frames / Second = " + toString(mStatisticsNumFrames) + "\n" + 
            "Time / Update " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

            mStatisticsUpdateTime -= sf::seconds(1.0f);
            mStatisticsNumFrames = 0;
        }
    }

```

The code above will take in an elapsedTime every run cycle and only update our frames when it has been greater than one second. We then subtract the time by a second and reset numFrames back to 0.


### Resource Management

Resources are typically heavyweight items such as images, music, or fonts. Meaning that the objects occupy a lot of memory and performing operations on them is computationally expensive. We need to think carefully how we store, and manipulate them in our programs.

In SFML, most resources have the same function call loadFromFile. Becuase of this we can create a templated class that will be able to store Textures, Fonts, Images, and Shaders and we dont have to write individual classes for each one. This greatly reduces how much code we have to write.

#### Relationship between Game Entities and Resources (textures, and fonts)

Game entities such as players, enemies, and the landscapes are represented with sprites. It's important to note that they do NOT own the heavy textures and fonts. Instead they just reference the heavy components. Sprites and sounds are considered the light weight front end classes.

This means that the resources need to be accessible by the entities. We must make sure that the resource objects stay alive as long as any front-end object refers to them.

In conclusion, the heavy resource class that we need to create will load all resources in at initialize time. This container class will need to be available to all game entities that need to load such a resource. This information will be delegated thourgh the constructor of the game entity. This way throughout their lifetime, front end classes such as sprites or sounds will be able to reference them.

#### ResourceHolder Class

Here is a link to our templated container class [ResourceHolder](../src/ResourceHolder.inl). The following SFML classes can make use of this class SF::Textures, SF::Shader, SF::Fonts, SF::Sound. Note that there is an additional overloaded function for load. This is so SF::Shader can use our class. Shader deviates slightly from the API of the other resource classes because it can consist of a fragment, or vertex shader. Shader has two different loadFromFile definitions depending on the parameters given.

This class allows us to write the following code:

``` c++

    ResourceHolder<sf::Texture, Textures::ID> mTextures;

    mTextures.load(Textures::Airplane, "path/to/texture/file");

    // Now our game entity sprite can easily load this texture.
    mPlayerPlane.setTexture(mTextures.get(Textures::Airplane));
```

Our class stores the sf::Texture, Textures::ID pairing in a map data structure and we can make use of the STL features in C++11.

# Sequential Rendering
How are things going to be rendered to the screen and in what order? How can we easily keep track of this so that everytime we call draw, we know that the elements will be ordered in the correct order?

You could create a container of entitys then call draw on each entity... However, If we ever want to handle another entity relative to another then this container approach makes it difficult. The best way to layout entitys in relation on the screen is in a Tree data structure.

## SceneGraph
A Scene Graph will help us manage transforms in a user-friendly way. This is a Tree data structure consisting of multiple nodes, called Scene Nodes. Each Scene Node will store an object that is drawn to the screen. Each node may have an arbitrary amount of child nodes, which adapt to the transform of their parent node when rendered. The children only store the position, rotation, and scale relative to their parent

#### Ownership Semantics
The scene graph will own the scene nodes. Therefore it will be responsible for their lifetime and destruction. We want each node to store all its child nodes. If a node is destroyed, its children are destroyed with it. If the root is destroyed, the whole scene graph is torn down.

We CANNOT use a std::vector<SceneNode> since the element types must be complete types. We could use std::vector<SceneNode*> but then we would have to manage memory ourselves which we dont want to do.
So, we use std::vecotr<std::unique_ptr<SceneNode>> instead. 

#### SceneNode Class
Our SceneNode Class will have a container to its children SceneNodes. And a SceneNode pointer to its parent
Code:
``` c++
std::vector<std::unique_ptr<SceneNode>> mChildren
SceneNode* mParent
```

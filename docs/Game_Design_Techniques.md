# Game Design Techniques
Attempting to develop a 2D game has shown me that I dont know much about how basic games work. This document is intended to explain underlying concepts in game design that are new to me. 

## Content
- [Game Loops and Frames](./Game_Design_Techniques.md#game-loops-and-frames)
- [Resource Management](./Game_Design_Techniques.md#resource-management)
- [Sequential Rendering](./Game_Design_Techniques.md#sequential-rendering)

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

You could create a container of entitys then call draw on each entity... However, If we ever want to handle another entity relative to another then this container approach makes it difficult. The best way to layout entitys in relation on the screen is in a Tree data structure known as a Scene Graph.

## SceneGraph
A Scene Graph will help us manage transforms in a user-friendly way. This is a Tree data structure consisting of multiple nodes, called Scene Nodes. Each Scene Node will store an object that is drawn to the screen and it will be able to define how it draws itself depending on what object it is. Each node may have an arbitrary amount of child nodes, which adapt to the transform of their parent node when rendered. The children only store the position, rotation, and scale relative to their parent

#### Ownership Semantics
The scene graph will own the scene nodes. Therefore it will be responsible for their lifetime and destruction. We want each node to store all its child nodes. If a node is destroyed, its children are destroyed with it. If the root is destroyed, the whole scene graph is torn down.

We CANNOT use a std::vector<SceneNode> since the element types must be complete types. We could use std::vector<SceneNode*> but then we would have to manage memory ourselves which we dont want to do.
So, we use std::vector<std::unique_ptr<SceneNode>> instead. 

#### SceneNode Class
Our SceneNode Class will have a container to its children SceneNodes. And a SceneNode pointer to its parent.
Adding and removing Nodes from the Tree needs to be defined. Adding is relatively simple, however, removing will require more code. The class will also need a update function so that it can update the current node, then retrieve its children and update those as well. We will also need to hand the function our fixed timeStep. The code below describes the basic structure

Code:
``` c++
private:
    std::vector<std::unique_ptr<SceneNode>> mChildren
    SceneNode* mParent
public:
    void attachChild(std::unique_ptr<SceneNode> child);
    std::unique_ptr<SceneNode> detachChild(const SceneNode& node);
    void update(sf::Time timeStep);
```

The public functions will be how we interface with our Tree. In order to simplify the implementation of the update function it will be useful to break it into different functions. The code below describes that change.

``` c++
private:
    std::vector<std::unique_ptr<SceneNode>> mChildren
    SceneNode* mParent
public:
    void attachChild(std::unique_ptr<SceneNode> child);
    std::unique_ptr<SceneNode> detachChild(const SceneNode& node);
    void update(sf::Time timeStep);

private:
    virtual void updateCurrent(sf::Time timeStep);
    void updateChildren(sf::Time timeStep);
```

We make updateCurrent virtual because it will be dependent on the type of SceneNode we are processing. Different entitys will need to be updated differently depending on game state. UpdateChildren() will simply iterate over the mChildren vector and call update again it iterates over the entire Tree and updates each node appropriately.

``` c++
void SceneNode::updateChildren(sf::time timeStep)
{
    for(const std::unique_ptr<SceneNode>& child : mChildren)
    {
        child->update(timeStep);
    }
}
```
Our Class will also need to override the draw() function from the SFML Drawable class. We will use the same structure to draw each node and its children as we do with the updates.
``` c++
virtual void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const final;
{
    states.transform *= getTransform();

    drawCurrent(target, states);
    drawChildren(target, states);
}
```
NOTE: Each of the draw classes is labeled as private because SceneNode::draw() is actually called from inside the Drawable class which SceneNode inherits from. 

The above code gets an initial state of the root node. It will send this state to its children and compute the new state.transform based on the parent nodes. The child then updates itself with the newly computed transform and draws itself.

The code for both drawChildren() will be the same as updateChildren() that is call draw() again if it has a child in the mChildren std::vector. The implementation for drawCurrent will be provided from the derived classes depending on how it needs to be drawn.

Every frame we will call these functions from the root SceneNode that way we iterate each child correctly

There is a visualization of the Scene Graph [Here](../docs/Diagrams/SceneGraph.dio)

# Events and User Input
In SFML, there is the following event types
- Window events
- Joystick events
- Keyboard events
- Mouse events

There is documentation on SFMLs website that goes into great detail regarding these [events](https://www.sfml-dev.org/tutorials/2.6/window-events.php)

## Real time input state
A major restriction with events is that they report only once and it goes into the event data structure. Meaning you cannot continously ask them how the satte of the input devices looks RIGHT NOW. You must wait until the code executes where you pop events from the DS.

SFML makes input management easier and implements classes that allows you to check for REAL TIME events. 

Ex.
```c++
void Game::update(sf::Time elapsedTime)
{
    sf::vector2f movement(0.f, 0.f);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= PlayerSpeed;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += PlayerSpeed;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= PlayerSpeed;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += PlayerSpeed;

    mPlayer.move(movement * elapsedTime.asSeconds());
}

```
When do you use Events vs Real Time input? A good rule of thumb is if a state has changed, you should use events. If you want to know the current state, then you use the real-time functions

Ex.
```c++
if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    // WHILE the left mouse buttion is pressed do something specific
if(event.type == sf::Event::MouseButtonPressed)
    // When the left mouse button has been pressed, do something specific
```

## Separating input handling and Game Logic
How do we separate input handling and game logic? We need a communication system that way when an event happens we send our a message to everyone that is interested.

### Commands
We will create a Command construct to denote messages that are sent to various game objects. A command is able to alter the object and to issue orders such as moving an entity, firing a weapon, or triggering an explosion.

#### std::function 
This function is useful to create a Command Event Communication system, so its necessary to understand how it works.

std::function class is a general purpose polymorphic function wrapper. Instances of std::function can store, copy, and invoke any target: lambda expressions, binding expressions or other function objects, as well as pointers to member functions and pointers to data members.

It is a type of disposal object. This means that it erases the details of how some operations happen and provides a uniform runtime interface for them.

Basic Example:

```c++
#include <functional>

int square(int x) {
    return x * x;
}

int main()
{
    std::function<int(int)> fn = square;

    std::cout << fn(3) << std::endl;
}
```

Example 2:
```c++

int add(int a, int b)
{
    return a + b;
}

std::function<int(int, int)> adder1 = &add;

std::function<int(int)> increaser = std::bind(&add, _1, 1);
int increased = increaser(5);   // Same as add(5, 1)
```


#### Structure of Command
Our structure of command will look like the following
```c++

struct Command
{
    Command();
    std::function<void(SceneNode&, sf::time)> action
    unsigned int category;
};
```

Each command will have a function Object associated with it via action. It will also have a category value. 
These are the enum values of the Category that we are sending the command to.
Example of the Category Structure

```c++

namespace Category
{
    enum Type
    {
        None = 0,
        Scene = 1 << 0,
        PlayerAircraft = 1 << 1,
        AlliedAircraft = 1 << 2,
        EnemyAircraft = 1 << 3
    };
}
```
We are shifting a the first bit over for each consecutive value and we get this
Scene = 0001
PlayerAircraft = 0010
AlliedAircraft = 0100
EnemyAircraft = 1000

This way we can use the category value from our Command Struct and we can COMBINE the enum values and send the command to multiple category types

#### Setting up Commands
In order to send commands to someone we need to set up recievers to our game objects. In our world, Commands will be passed to our Scene Graph, which they are then distributed to all scene nodes with corresponding game objects
Each SceneNode will be responsible for forwarding a command to its children
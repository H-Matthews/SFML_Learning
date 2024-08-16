#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow{ { 480u, 760u }, "CMake SFML Project" };

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}

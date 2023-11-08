#include <SFML/Graphics.hpp>
#include "Renderer.hpp"
#include "Solver.h"

int main()
{
    constexpr int32_t window_width  = 1920;
    constexpr int32_t window_height = 1080;
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "PhysicsDemo", sf::Style::Default, settings);
    window.setFramerateLimit(144);


    Renderer renderer(window);
    Solver solver;

    solver.addObject({window_width / 2.0,0,},{1,0.0});
    solver.addObject({window_width / 2.0-100,0,},{-1,0.0});

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
        solver.update(0.01);
        renderer.render(solver);
        window.display();
    }
}
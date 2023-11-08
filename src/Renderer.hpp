#pragma once
#include "Solver.h"

class Renderer
{
public:
    explicit
    Renderer(sf::RenderTarget& target)
            : m_target{target}
    {

    }

    void render(const Solver& solver) const {
        // Render objects
        sf::CircleShape circle{1.0f};
        circle.setPointCount(32);
        circle.setOrigin(1.0f, 1.0f);
        const auto &objects = solver.getObjects();
        for (const auto &obj: objects) {
            circle.setPosition(obj.curr_pos);
            circle.setScale(obj.radius, obj.radius);
            circle.setFillColor(obj.color);
            m_target.draw(circle);
        }

        // Render the V floor
        sf::Vector2f vCenter = {0.0f, 600.0f}; // Center point of the V
        float vLength = 1400.0f; // Length of each line from the center


        sf::VertexArray vFloor(sf::Lines, 2);

        // Calculate the direction vectors for the left and right lines of the V
        // Since SFML's y-coordinate increases downwards, we subtract the sin() component to go up on the screen
        sf::Vector2f leftDirection(1, 0.6);

        // Set positions for the left line
        vFloor[0].position = {0.0f, 900.0f}; // Center point of the V
        vFloor[1].position ={700.0f, 1001.0f}; // Center point of the V


        // Draw the V floor
        m_target.draw(vFloor);

        // Render horizontal floor
        sf::VertexArray hFloor(sf::Lines, 2);
        hFloor[0].position = {0,1000}; // Start at center
        hFloor[1].position = {1920,1000}; // End point calculated by direction * length
        // Draw the H floor
        m_target.draw(hFloor);




        // Set color for both lines
        for (int i = 0; i < 2; ++i) {
            vFloor[i].color = sf::Color::White; // or any color you want for the floor
        }

        for (int i = 0; i < 2; ++i) {
            hFloor[i].color = sf::Color::White; // or any color you want for the floor
        }

    }



private:
    sf::RenderTarget& m_target;
};
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
        sf::Vector2f vCenter = {800.0f, 1200.0f}; // Center point of the V
        float vAngle = 45.0f; // Angle from vertical in degrees for each line
        float vLength = 1000.0f; // Length of each line from the center

        sf::VertexArray vFloor(sf::Lines, 4);

        // Calculate the direction vectors for the left and right lines of the V
        // Since SFML's y-coordinate increases downwards, we subtract the sin() component to go up on the screen
        sf::Vector2f leftDirection(-0.785398, -0.785398);
        sf::Vector2f rightDirection(0.785398, -0.785398);

        // Set positions for the left line
        vFloor[0].position = vCenter; // Start at center
        vFloor[1].position = vCenter + (leftDirection * vLength); // End point calculated by direction * length
        // Set positions for the right line
        vFloor[2].position = vCenter; // Start at center
        vFloor[3].position = vCenter + (rightDirection * vLength); // End point calculated by direction * length



        // Draw the V floor
        m_target.draw(vFloor);

        // Render horizontal floor
        sf::VertexArray hFloor(sf::Lines, 2);
        hFloor[0].position = {0,1000}; // Start at center
        hFloor[1].position = {1920,1000}; // End point calculated by direction * length
        // Draw the H floor
        m_target.draw(hFloor);
        // Set color for both lines
        for (int i = 0; i < 4; ++i) {
            vFloor[i].color = sf::Color::White; // or any color you want for the floor
        }
        // Set color for both lines
        for (int i = 0; i < 2; ++i) {
            hFloor[i].color = sf::Color::White; // or any color you want for the floor
        }
    }



private:
    sf::RenderTarget& m_target;
};
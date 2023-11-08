#pragma once
#include <SFML/Graphics.hpp>

struct Object
{
    sf::Vector2f curr_pos;
    sf::Vector2f old_pos;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    float        mass          = 1.0f;
    float        radius        = 10.0f;
    sf::Color    color         = sf::Color::White;

    Object() = default;

    explicit Object(sf::Vector2f pos, sf::Vector2f velocity)
        : curr_pos{pos}
        , old_pos{pos}
        , velocity{velocity}
    {}

    void updatePosition(float dt)
    {
        old_pos = curr_pos;
        curr_pos = curr_pos + velocity + acceleration * dt * dt;
        velocity = curr_pos - old_pos;
        acceleration = {};
    }
    void accelerate(sf::Vector2f acc)
    {
        acceleration += acc;
    }
};
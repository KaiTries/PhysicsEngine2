#pragma once
#include <math.h>
#include "object.h"
#include <SFML/Graphics.hpp>
#include "util/maths.hpp"

struct Solver
{

    sf::Vector2f gravity = {0.0f,1000.0f};
    std::vector<Object> objects;



    // Const version for const Solver objects
    [[nodiscard]]
    const std::vector<Object>& getObjects() const {
        return objects;
    }

    void addObject(sf::Vector2f pos, sf::Vector2f acc)
    {
        objects.emplace_back(pos,acc);
    }

    void update(float dt)
    {
        applyGravity();
        applyConstraint();
        updatePositions(dt);
    }

    void updatePositions(float dt)
    {
        for (auto& object : objects) {
            object.updatePosition(dt);
        }

    }
    void applyGravity()
    {
        for (auto& object : objects) {
            object.accelerate(gravity);
        }
    }


    void applyConstraint()
    {
        const float restitution = 0.8; // Value between 0 and 1 where 1 is a perfect bounce and 0 no bounce at all.
        const float leftWallX = 0.0f; // X coordinate of the left wall
        const float rightWallX = 1920.0f; // X coordinate of the right wall (replace with your actual value)
        const float coefficientOfFriction = 0.001f; // Coefficient of friction between the ball and the floor

        // Render the _ floor
        sf::Vector2f fstart = {0.0f, 1000.0f}; // Center point of the V
        sf::Vector2f fend = {1920.0f, 1000.0f}; // Center point of the V

        // Set positions for the left line
        sf::Vector2f  start = {0.0f, 900.0f}; // Center point of the V
        sf::Vector2f  end = {700.0f, 1001.0f}; // End point calculated by direction * length


        for (auto& obj : objects) {

            // Apply constraints as previously described
            auto [distance, closestPoint] = pointLineDistanceAndClosestPoint(obj.curr_pos, start, end);

            auto [fdistance, fclosestPoint] = pointLineDistanceAndClosestPoint(obj.curr_pos, fstart, fend);


            if (distance < obj.radius) {
                sf::Vector2f collisionNormal = obj.curr_pos - closestPoint;
                float magnitude = std::sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);
                sf::Vector2f normalizedCollisionNormal = collisionNormal / magnitude;
                obj.curr_pos = closestPoint + normalizedCollisionNormal * obj.radius;
                // Reflect velocity
                // Calculate the dot product of velocity and the collision normal
                float dotProduct = obj.velocity.x * normalizedCollisionNormal.x + obj.velocity.y * normalizedCollisionNormal.y;
                // Reflect the velocity vector
                obj.velocity -= 2 * restitution * dotProduct * normalizedCollisionNormal;
            }

            if (fdistance < obj.radius) {
                sf::Vector2f collisionNormal = obj.curr_pos - fclosestPoint;
                float magnitude = std::sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);
                sf::Vector2f normalizedCollisionNormal = collisionNormal / magnitude;
                obj.curr_pos = fclosestPoint + normalizedCollisionNormal * obj.radius;
                float dotProduct = obj.velocity.x * normalizedCollisionNormal.x + obj.velocity.y * normalizedCollisionNormal.y;
                obj.velocity -= 2 * restitution * dotProduct * normalizedCollisionNormal;

                // Apply friction if the object is in contact with the floor
                // Friction force is opposite to the direction of motion
                if (obj.velocity.x != 0.0f) { // Check if the object is moving horizontally
                    float frictionForce = coefficientOfFriction * obj.mass * 9.81f; // mass * gravity * coefficientOfFriction
                    float frictionAcceleration = frictionForce / obj.mass; // a = F/m

                    // Apply friction acceleration in the opposite direction to the velocity
                    if (obj.velocity.x > 0) { // Object moving to the right
                        obj.velocity.x -= frictionAcceleration;
                        if (obj.velocity.x < 0) obj.velocity.x = 0; // Prevent the object from starting to move in the opposite direction
                    } else { // Object moving to the left
                        obj.velocity.x += frictionAcceleration;
                        if (obj.velocity.x > 0) obj.velocity.x = 0;
                    }
                }
            }
            // Apply left wall constraint
            if (obj.curr_pos.x - obj.radius < leftWallX) {
                obj.curr_pos.x = leftWallX + obj.radius;
                obj.velocity.x = -obj.velocity.x * restitution;
            }

            // Apply right wall constraint
            if (obj.curr_pos.x + obj.radius > rightWallX) {
                obj.curr_pos.x = rightWallX - obj.radius;
                obj.velocity.x = -obj.velocity.x * restitution;
            }
        }
    }
};
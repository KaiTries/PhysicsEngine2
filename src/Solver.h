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
        const float floorY = 1000.0f; // Y coordinate of the floor
        const float leftWallX = 0.0f; // X coordinate of the left wall
        const float rightWallX = 1920.0f; // X coordinate of the right wall (replace with your actual value)
        const float coefficientOfFriction = 0.001f; // Coefficient of friction between the ball and the floor


        // Render the V floor
        sf::Vector2f vCenter = {800.0f, 1200.0f}; // Center point of the V
        float vAngle = 45.0f; // Angle from vertical in degrees for each line
        float angleInRadians = vAngle * (3.14159265f / 180.0f);
        float vLength = 1000.0f; // Length of each line from the center

        sf::VertexArray vFloor(sf::Lines, 4);

        // Calculate the direction vectors for the left and right lines of the V
        // Since SFML's y-coordinate increases downwards, we subtract the sin() component to go up on the screen
        sf::Vector2f leftDirection(-0.785398, 0.785398);
        sf::Vector2f rightDirection(0.785398, 0.785398);

        // Set positions for the left line
        vFloor[0].position = vCenter; // Start at center
        vFloor[1].position = vCenter + (leftDirection * vLength); // End point calculated by direction * length
        // Set positions for the right line
        vFloor[2].position = vCenter; // Start at center
        vFloor[3].position = vCenter + (rightDirection * vLength); // End point calculated by direction * length





        for (auto& obj : objects) {

            // Apply constraints as previously described
            auto [distToLeft, closestPointToLeft] = pointLineDistanceAndClosestPoint(obj.curr_pos, vCenter, vFloor[1].position);
            auto [distToRight, closestPointToRight] = pointLineDistanceAndClosestPoint(obj.curr_pos, vCenter, vFloor[3].position);

            if (distToLeft < obj.radius) {
                sf::Vector2f collisionNormal = obj.curr_pos - closestPointToLeft;
                float magnitude = std::sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);
                sf::Vector2f normalizedCollisionNormal = collisionNormal / magnitude;
                obj.curr_pos = closestPointToLeft + normalizedCollisionNormal * obj.radius;
                // Reflect velocity
                // Calculate the dot product of velocity and the collision normal
                float dotProduct = obj.velocity.x * normalizedCollisionNormal.x + obj.velocity.y * normalizedCollisionNormal.y;

                // Reflect the velocity vector
                obj.velocity -= 2 * dotProduct * normalizedCollisionNormal;
            }

            if (distToRight < obj.radius) {
                sf::Vector2f collisionNormal = obj.curr_pos - closestPointToRight;
                float magnitude = std::sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);
                sf::Vector2f normalizedCollisionNormal = collisionNormal / magnitude;
                obj.curr_pos = closestPointToRight + normalizedCollisionNormal * obj.radius;
                float dotProduct = obj.velocity.x * normalizedCollisionNormal.x + obj.velocity.y * normalizedCollisionNormal.y;
                // Reflect the velocity vector
                obj.velocity -= 2 * dotProduct * normalizedCollisionNormal;
            }

            // Apply floor constraint
            if (obj.curr_pos.y + obj.radius > floorY) {
                obj.curr_pos.y = floorY - obj.radius;
                obj.velocity.y = 0.0;

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
                obj.velocity.x = 0; // Stop the ball when it hits the wall
            }

            // Apply right wall constraint
            if (obj.curr_pos.x + obj.radius > rightWallX) {
                obj.curr_pos.x = rightWallX - obj.radius;
                obj.velocity.x = 0; // Stop the ball when it hits the wall
            }
        }
    }
};
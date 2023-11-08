#include <SFML/Graphics.hpp>
#include <math.h>

float dot(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Function to calculate distance from a point to a line, and the closest point on the line
std::pair<float, sf::Vector2f> pointLineDistanceAndClosestPoint(const sf::Vector2f& point, const sf::Vector2f& lineStart, const sf::Vector2f& lineEnd) {
    sf::Vector2f lineDirection = lineEnd - lineStart;
    sf::Vector2f lineToBall = point - lineStart;

    // Project lineToBall onto lineDirection, normalize lineDirection first
    sf::Vector2f normalizedLineDirection = lineDirection / std::sqrt(lineDirection.x * lineDirection.x + lineDirection.y * lineDirection.y);
    float projectionLength = dot(lineToBall, normalizedLineDirection);
    sf::Vector2f closestPoint = lineStart + normalizedLineDirection * projectionLength;
    sf::Vector2f distanceVector = point - closestPoint;

    return {std::sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y), closestPoint};
}
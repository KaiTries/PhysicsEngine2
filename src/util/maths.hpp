#include <SFML/Graphics.hpp>
#include <math.h>

float dot(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Function to calculate distance from a point to a line, and the closest point on the line
std::pair<float, sf::Vector2f> pointLineDistanceAndClosestPoint(const sf::Vector2f& point, const sf::Vector2f& lineStart, const sf::Vector2f& lineEnd) {
    sf::Vector2f lineDirection = lineEnd - lineStart;
    sf::Vector2f normalizedLineDirection = lineDirection / std::sqrt(lineDirection.x * lineDirection.x + lineDirection.y * lineDirection.y);
    sf::Vector2f lineToBall = point - lineStart;
    float projectionLength = dot(lineToBall, normalizedLineDirection);

    // Clamp the projection length to the range [0, length of the line segment]
    float lineSegmentLength = std::sqrt(lineDirection.x * lineDirection.x + lineDirection.y * lineDirection.y);
    projectionLength = std::max(0.f, std::min(projectionLength, lineSegmentLength));

    // Calculate the closest point based on the clamped projection
    sf::Vector2f closestPoint = lineStart + normalizedLineDirection * projectionLength;
    sf::Vector2f distanceVector = point - closestPoint;

    return {std::sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y), closestPoint};
}
#include "Colliders.hpp"

GameEngine::Collider::~Collider() {}

glm::vec3& GameEngine::Collider::getPosition() const
{
    return glm::vec3(position);
}

void GameEngine::Collider::setPosition(const glm::vec3& pos)
{
    position = pos;
}

GameEngine::BoxCollider::BoxCollider(const int id, const glm::vec3& pos, const glm::vec3& dim)
{
    gameObjectID = id;
    position = position;
    dimensions = dim;
}

glm::vec3& GameEngine::BoxCollider::getDimensions() const
{
    return glm::vec3(dimensions);
}

void GameEngine::BoxCollider::setDimensions(const glm::vec3& dim)
{
    dimensions = dim;
}

GameEngine::SphereCollider::SphereCollider(const int id, const glm::vec3& pos, const double rad)
{
    gameObjectID = id;
    position = pos;
    radius = rad;
}

double GameEngine::SphereCollider::getRadius() const
{
    return radius;
}

void GameEngine::SphereCollider::setRadius(const double rad)
{
    radius = rad;
}
#pragma once
#include "../config.h"
#include "../components/transform_component.h"
#include "../components/physics_component.h"

class CollisionSystem {
public:
    void update(
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        std::unordered_map<unsigned int,PhysicsComponent> &physicsComponents);
};
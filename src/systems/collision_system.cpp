#include "collision_system.h"

void CollisionSystem::update(
    std::unordered_map<unsigned int,TransformComponent> &transformComponents,
    std::unordered_map<unsigned int,PhysicsComponent> &physicsComponents) {

    std::vector<unsigned int> collidableEntities;
    collidableEntities.reserve(physicsComponents.size());

    for (auto& [entity, physics] : physicsComponents) {
        if (physics.radius > 0.0f) {
            collidableEntities.push_back(entity);
        }
    }

    for (size_t i = 0; i < collidableEntities.size(); ++i) {
        for (size_t j = i + 1; j < collidableEntities.size(); ++j) {

            unsigned int entityA = collidableEntities[i];
            unsigned int entityB = collidableEntities[j];

            if (transformComponents.find(entityA) == transformComponents.end() ||
                transformComponents.find(entityB) == transformComponents.end()) {
                continue;
            }

            PhysicsComponent& physicsA = physicsComponents[entityA];
            PhysicsComponent& physicsB = physicsComponents[entityB];
            TransformComponent& transformA = transformComponents[entityA];
            TransformComponent& transformB = transformComponents[entityB];

            glm::vec3 delta = transformB.position - transformA.position;
            float distance = glm::length(delta);
            float minDistance = physicsA.radius + physicsB.radius;

            if (distance >= minDistance) {
                continue;
            }

            glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f);
            if (distance > 0.0001f) {
                normal = delta / distance;
            } else {
                glm::vec3 relativeVelocity = physicsA.velocity - physicsB.velocity;
                if (glm::length(relativeVelocity) > 0.0001f) {
                    normal = glm::normalize(relativeVelocity);
                }
            }

            float overlap = minDistance - distance;
            transformA.position -= normal * (overlap * 0.5f);
            transformB.position += normal * (overlap * 0.5f);

            glm::vec3 relativeVelocity = physicsA.velocity - physicsB.velocity;
            float velocityAlongNormal = glm::dot(relativeVelocity, normal);

            if (velocityAlongNormal < 0.0f) {
                physicsA.velocity = physicsA.velocity - 2.0f * glm::dot(physicsA.velocity, normal) * normal;
                physicsB.velocity = physicsB.velocity - 2.0f * glm::dot(physicsB.velocity, normal) * normal;
            }
        }
    }
}
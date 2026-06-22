#pragma once
#include "../config.h"

struct PhysicsComponent {
    glm::vec3 velocity;
    glm::vec3 eulerVelocity;
    float radius = 0.0f;
};
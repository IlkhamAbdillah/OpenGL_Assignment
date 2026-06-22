#pragma once
#include "../config.h"
#include "../components/camera_component.h"
#include "../components/physics_component.h"
#include "../components/transform_component.h"

class CameraSystem {
public:

    CameraSystem(unsigned int shader, GLFWwindow* window);
    
    bool update(
        std::unordered_map<unsigned int,TransformComponent> &transformComponents,
        std::unordered_map<unsigned int,PhysicsComponent> &physicsComponents,
        unsigned int cameraID, CameraComponent& cameraComponent, float dt);
    
private:
    unsigned int viewLocation;
    glm::vec3 global_up = {0.0f, 0.0f, 3.0f};
    GLFWwindow* window;
    
    // Jump system
    float verticalVelocity = 0.0f;
    bool isJumping = false;
    const float JUMP_FORCE = 2.0f;
    const float GRAVITY = 0.98f;
    bool spacePressed = false;
};
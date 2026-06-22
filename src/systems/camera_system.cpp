#include "camera_system.h"

CameraSystem::CameraSystem(unsigned int shader, GLFWwindow* window) {
    this->window = window;

    glUseProgram(shader);
    viewLocation = glGetUniformLocation(shader, "view");
}

bool CameraSystem::update(
    std::unordered_map<unsigned int,TransformComponent> &transformComponents,
    std::unordered_map<unsigned int,PhysicsComponent> &physicsComponents,
    unsigned int cameraID, CameraComponent& cameraComponent, float dt) {

    glm::vec3& pos = transformComponents[cameraID].position;
    glm::vec3& eulers = transformComponents[cameraID].eulers;
    float theta = glm::radians(eulers.z);
    float phi = glm::radians(eulers.y);

    glm::vec3& right = cameraComponent.right;
    glm::vec3& up = cameraComponent.up;
    glm::vec3& forwards = cameraComponent.forwards;

    forwards = {
        glm::cos(theta) * glm::cos(phi),
        glm::sin(theta) * glm::cos(phi),
        glm::sin(phi)
    };
    right = glm::normalize(glm::cross(forwards, global_up));
    up = glm::normalize(glm::cross(right, forwards));

    glm::mat4 view = glm::lookAt(pos, pos + forwards, up);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    //Keys
    glm::vec3 dPos = {0.0f, 0.0f, 0.0f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        dPos.x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        dPos.y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        dPos.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        dPos.y += 1.0f;
    }
    if (glm::length(dPos) > 0.1f) {
        dPos = glm::normalize(dPos);
        
        // Horizontal movement only (ignore Z component)
        glm::vec3 horizontalForwards = glm::normalize(glm::vec3(forwards.x, forwards.y, 0.0f));
        glm::vec3 horizontalRight = glm::normalize(glm::vec3(right.x, right.y, 0.0f));

        glm::vec3 proposedPos = pos;
        proposedPos += 0.01f * dPos.x * horizontalForwards;
        proposedPos += 0.01f * dPos.y * horizontalRight;

        const float cameraRadius = 0.25f;
        for (const auto& [entityID, physics] : physicsComponents) {
            if (entityID == cameraID || physics.radius <= 0.0f) {
                continue;
            }

            auto transformIt = transformComponents.find(entityID);
            if (transformIt == transformComponents.end()) {
                continue;
            }

            glm::vec3 obstaclePos = transformIt->second.position;
            glm::vec3 delta = proposedPos - obstaclePos;
            float distance = glm::length(delta);
            float minDistance = cameraRadius + physics.radius;

            if (distance >= minDistance) {
                continue;
            }

            glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f);
            if (distance > 0.0001f) {
                normal = delta / distance;
            } else {
                glm::vec3 fallback = glm::normalize(glm::vec3(forwards.x, forwards.y, 0.0f));
                if (glm::length(fallback) < 0.0001f) {
                    fallback = glm::normalize(glm::vec3(right.x, right.y, 0.0f));
                }
                if (glm::length(fallback) < 0.0001f) {
                    fallback = glm::vec3(1.0f, 0.0f, 0.0f);
                }
                normal = fallback;
            }

            proposedPos = obstaclePos + normal * minDistance;
        }

        pos = proposedPos;
    }

    // Clamp z to not go below 1.5
    if (pos.z < 1.5f) {
        pos.z = 1.5f;
    }

    bool spacePressedNow = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (spacePressedNow && !spacePressed && !isJumping) {
        isJumping = true;
        verticalVelocity = JUMP_FORCE;
    }
    spacePressed = spacePressedNow;

    if (isJumping) {
        pos.z += verticalVelocity * dt;
        verticalVelocity -= GRAVITY * dt;

        if (verticalVelocity < 0.0f && pos.z <= 1.5f) {
            pos.z = 1.5f;
            verticalVelocity = 0.0f;
            isJumping = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return true;
    }

    //Mouse
    glm::vec3 dEulers = {0.0f, 0.0f, 0.0f};
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    glfwSetCursorPos(window, 960.0, 540.0);
    glfwPollEvents();

    dEulers.z = -0.05f * static_cast<float>(mouse_x - 960.0);
    dEulers.y = -0.05f * static_cast<float>(mouse_y - 540.0);

    eulers.y = fminf(89.0f, fmaxf(-89.0f, eulers.y + dEulers.y));

    eulers.z += dEulers.z;
    if (eulers.z > 360) {
        eulers.z -= 360;
    }
    else if (eulers.z < 0) {
        eulers.z += 360;
    }

    return false;
}
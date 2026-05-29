#include "render_system.h"

RenderSystem::RenderSystem(unsigned int shader, GLFWwindow* window) {
    
    modelLocation = glGetUniformLocation(shader, "model");
    this->window = window;
}
    
void RenderSystem::update(
    std::unordered_map<unsigned int,TransformComponent> &transformComponents,
    std::unordered_map<unsigned int,RenderComponent> &renderComponents) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (auto& [entity,renderable] : renderComponents) {

        TransformComponent& transform = transformComponents[entity];
        glm::mat4 model = glm::mat4(1.0f);
	    model = glm::translate(model, transform.position);
	    model = glm::rotate(
            model, glm::radians(transform.eulers.z), 
            { 0.0f, 0.0f, 1.0f });
        glUniformMatrix4fv(
		    modelLocation, 1, GL_FALSE, 
		    glm::value_ptr(model));

        if (renderable.materials.empty() || renderable.VAOs.empty() || renderable.vertexCounts.empty()) {
            glBindTexture(GL_TEXTURE_2D, renderable.material);
            glBindVertexArray(renderable.VAO);
	        glDrawArrays(GL_TRIANGLES, 0, renderable.vertexCount);
            continue;
        }

        for (size_t i = 0; i < renderable.VAOs.size(); ++i) {
            glBindTexture(GL_TEXTURE_2D, renderable.materials[i]);
            glBindVertexArray(renderable.VAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, renderable.vertexCounts[i]);
        }
    }
	glfwSwapBuffers(window);
}
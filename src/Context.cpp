#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mantaray/OpenGL/Context.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

bool Context::IsInitialized = false;

bool Context::Create(GLFWwindow** outWindow, std::string title, Vector2u size) {
    if (!Context::IsInitialized) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        *outWindow = glfwCreateWindow((int)size.x, (int)size.y, title.c_str(), NULL, NULL);
        if (*outWindow == NULL)
        {
            Logger::Log("Context", "Failed to create GLFW window", MR::Logger::LOG_ERROR);
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(*outWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::Log("Context", "Failed to initialize GLAD", MR::Logger::LOG_ERROR);
            return false;
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        Context::IsInitialized = true;
        return true;
    } 
    else {
        Logger::Log("Context", "Context is already initialized", Logger::LOG_WARNING);
        return false;
    }
}

void Context::Destroy() {
    if (Context::IsInitialized) {
        glfwTerminate();
        Context::IsInitialized = false;
    } 
    else {
        Logger::Log("Context", "Context is not initialized", Logger::LOG_WARNING);
    }
}

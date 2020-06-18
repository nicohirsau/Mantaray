#pragma once

#include <string>

#include "Mantaray/Core/Vector.hpp"

namespace MR {
struct GLState {
    unsigned int boundTextureID = -1;
    unsigned int boundRenderTextureID = -1;
    unsigned int boundVertexArrayObjectID = -1;
    unsigned int boundShaderProgramID = -1;
};

class Context {
    public:
        static bool Create(class GLFWwindow** outWindow, std::string title, Vector2u size);
        static void Destroy();

        static void BindTexture(unsigned int textureID);
        static void BindFramebuffer(unsigned int renderTextureID);
        static void BindVertexArray(unsigned int vertexArrayObjectID);
        static void UseProgram(unsigned int shaderProgramID);
    
    private:
        static bool IsInitialized;
        static GLState State; 
};
}
#pragma once

#include <string>

#include "Mantaray/Core/Vector.hpp"

namespace MR {
struct GLState {
    unsigned int boundTextureID = -1;
    unsigned int boundFrameBufferID = -1;
    unsigned int boundVertexArrayID = -1;
    unsigned int boundShaderProgramID = -1;
};

class Context {
    public:
        static bool Create(class GLFWwindow** outWindow, std::string title, Vector2u size);
        static void Destroy();

        static void BindTexture2D(unsigned int textureID);
        static void BindFramebuffer(unsigned int frameBufferID);
        static void BindVertexArray(unsigned int vertexArrayID);
        static void UseProgram(unsigned int shaderProgramID);
    
    private:
        static bool IsInitialized;
        static GLState State; 
};
}
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#pragma once

#include "Mantaray/Core/Vector.h"
#include "Mantaray/GLObjects/GLObject.h"

namespace MR {
class Texture;

class Shader : public GLObject {
    public:
        enum ShaderType {
            VERTEX_SHADER,
            FRAGMENT_SHADER
        };

        Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath);
        ~Shader();

        void activate();
        void setupForDraw();

        void setUniformInteger(std::string uniformName, int value);
        void setUniformFloat(std::string uniformName, float value);
        void setUniformVector2f(std::string uniformName, Vector2<float> value);
        void setUniformVector3f(std::string uniformName, Vector3<float> value);
        void setUniformMatrix4(std::string uniformName, glm::mat4 value);
        void setTexture(std::string textureUniformName, int slot, Texture &texture);
        
        static unsigned int CompileShader(Shader::ShaderType shaderType, const char* source);
        static unsigned int LinkShader(unsigned int vertexShader, unsigned int fragmentShader);

    protected:
        void allocate() override;
        void release() override;

    private:
        void compileShader(Shader::ShaderType shaderType, const char* source);
        void linkShader();
        int getUniformLocation(std::string uniformName);

    private:
        unsigned int m_FragmentShaderID, m_VertexShaderID, m_ShaderProgramID;
        std::unordered_map<std::string, int> m_Uniforms = std::unordered_map<std::string, int>();
        std::unordered_map<int, unsigned int> m_TextureSlots = std::unordered_map<int, unsigned int>();
};
}

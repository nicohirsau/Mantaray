#include <string>
#include <unordered_map>

#include "Mantaray/Core/Vector.h"

namespace MR {
class Texture;

class Shader {
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
        void setUniformVector2f(std::string uniformName, Vector2f value);
        void setUniformVector3f(std::string uniformName, Vector3f value);
        void setTexture(std::string textureUniformName, int slot, Texture &texture);

        static unsigned int CompileShader(Shader::ShaderType shaderType, const char* source);
        static unsigned int LinkShader(unsigned int vertexShader, unsigned int fragmentShader);

    private:
        int getUniformLocation(std::string uniformName);

    private:
        unsigned int m_FragmentShaderID, m_VertexShaderID, m_ShaderProgramID;
        std::unordered_map<std::string, int> m_Uniforms = std::unordered_map<std::string, int>();
        std::unordered_map<int, unsigned int> m_TextureSlots = std::unordered_map<int, unsigned int>();
};
}

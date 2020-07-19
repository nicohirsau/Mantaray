#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mantaray/OpenGL/Context.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/Objects/Texture.hpp"
#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"
#include "Mantaray/Core/FileSystem.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    link();
    compileShader(VERTEX_SHADER, vertexShaderSource);
    compileShader(FRAGMENT_SHADER, fragmentShaderSource);
    linkShader();
}

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
    link();
    std::string vertexShaderContent;
    FileSystem::ReadFile(vertexShaderPath, vertexShaderContent);
    const char* vertexShaderSource = vertexShaderContent.c_str();
    
    std::string fragmentShaderContent;
    FileSystem::ReadFile(fragmentShaderPath, fragmentShaderContent);
    const char* fragmentShaderSource = fragmentShaderContent.c_str();

    compileShader(VERTEX_SHADER, vertexShaderSource);
    compileShader(FRAGMENT_SHADER, fragmentShaderSource);
    linkShader();
}

Shader::~Shader() {
    unlink();
}

void Shader::allocate() {
    m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    m_ShaderProgramID = glCreateProgram();
}

void Shader::release() {
    glDeleteShader(m_VertexShaderID);
    glDeleteShader(m_FragmentShaderID);
    glDeleteProgram(m_ShaderProgramID);
}

void Shader::bind() {
    Context::UseProgram(m_ShaderProgramID);
}

void Shader::unbind() {
    Context::UseProgram(0);
}

void Shader::setupForDraw() {
    bind();
    for (auto& texture_slot: m_TextureSlots) {
        glActiveTexture(0x84C0 + texture_slot.first);
        Context::BindTexture2D(texture_slot.second);
    }
}

void Shader::setUniformInteger(std::string uniformName, int value) {
    bind();
    int uniformLocation = getUniformLocation(uniformName);
    glUniform1i(uniformLocation, value);
}

void Shader::setUniformFloat(std::string uniformName, float value) {
    bind();
    int uniformLocation = getUniformLocation(uniformName);
    glUniform1f(uniformLocation, value);
}

void Shader::setUniformVector2f(std::string uniformName, Vector2f value) {
    bind();
    int uniformLocation = getUniformLocation(uniformName);
    glUniform2f(uniformLocation, value.x, value.y);
}

void Shader::setUniformVector3f(std::string uniformName, Vector3f value) {
    bind();
    int uniformLocation = getUniformLocation(uniformName);
    glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Shader::setUniformVector4f(std::string uniformName, Vector4f value) {
    bind();
    int uniformLocation = getUniformLocation(uniformName);
    glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
}

void Shader::setUniformMatrix4(std::string uniformName, glm::mat4 value) {
    bind();
    int uniformLocation = getUniformLocation(uniformName);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setTexture(std::string textureUniformName, int slot, Texture &texture) {
    if (slot > 31) {
        Logger::Log(
            "Shader", "Texture slot " + std::to_string(slot) + " is over the limit of 31!", 
            Logger::LOG_WARNING
        );
        return;
    }
    bind();
    glActiveTexture(0x84C0 + slot);
    Context::BindTexture2D(texture.getTextureID());
    setUniformInteger(textureUniformName, slot);
    m_TextureSlots[slot] = texture.getTextureID();
}

void Shader::setRenderTexture(std::string textureUniformName, int slot, RenderTexture &texture) {
    if (slot > 31) {
        Logger::Log(
            "Shader", "Texture slot " + std::to_string(slot) + " is over the limit of 31!", 
            Logger::LOG_WARNING
        );
        return;
    }
    bind();
    glActiveTexture(0x84C0 + slot);
    Context::BindTexture2D(texture.m_RenderTexture->getTextureID());
    setUniformInteger(textureUniformName, slot);
    m_TextureSlots[slot] = texture.m_RenderTexture->getTextureID();
}

int Shader::getUniformLocation(std::string uniformName) {
    int uniformLocation = -1;

    std::unordered_map<std::string, int>::iterator it;
    it = m_Uniforms.find(uniformName);

    if (it == m_Uniforms.end()) {
        uniformLocation = glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
        if (uniformLocation == -1) {
            Logger::Log("Shader", "Cannot find uniform location for: " + uniformName, Logger::LOG_WARNING);
        }
        m_Uniforms[uniformName] = uniformLocation;
    } else {
        uniformLocation = it->second;
    }
    return uniformLocation;
}

void Shader::compileShader(Shader::ShaderType shaderType, const char* source) {
    unsigned int shaderID;
    switch (shaderType)
    {
    case Shader::VERTEX_SHADER:
        shaderID = m_VertexShaderID;
        glShaderSource(m_VertexShaderID, 1, &source, NULL);
        glCompileShader(m_VertexShaderID);
        break;
    case Shader::FRAGMENT_SHADER:
        shaderID = m_FragmentShaderID;
        glShaderSource(m_FragmentShaderID, 1, &source, NULL);
        glCompileShader(m_FragmentShaderID);
        break;
    
    default:
        return;
    }

    int  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::string s_ShaderType = (shaderType == VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::string message = s_ShaderType + "::COMPILING_SHADER_FAILED\n";
        message.append(infoLog);
        Logger::Log("ShaderCompiler", message, Logger::LOG_ERROR);
        return;
    }
}

void MR::Shader::linkShader() {
    glAttachShader(m_ShaderProgramID, m_VertexShaderID);
    glAttachShader(m_ShaderProgramID, m_FragmentShaderID);
    glLinkProgram(m_ShaderProgramID);
    
    int  success;
    char infoLog[512];
    glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_ShaderProgramID, 512, NULL, infoLog);
        std::string message = "LINKING_SHADER_FAILED\n";
        message.append(infoLog);
        Logger::Log("ShaderLinker", message, Logger::LOG_ERROR);
        return;
    }
}

unsigned int Shader::CompileShader(Shader::ShaderType shaderType, const char* source) {
    unsigned int shaderID;

    switch (shaderType)
    {
    case Shader::VERTEX_SHADER:
        shaderID = glCreateShader(GL_VERTEX_SHADER);
        break;
    case Shader::FRAGMENT_SHADER:
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    
    default:
        return -1;
    }

    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::string s_ShaderType = (shaderType == VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        std::string message = s_ShaderType + "::COMPILING_SHADER_FAILED\n";
        message.append(infoLog);
        Logger::Log("ShaderCompiler", message, Logger::LOG_ERROR);
        return -1;
    }

    return shaderID;
}

unsigned int MR::Shader::LinkShader(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    
    int  success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::string message = "LINKING_SHADER_FAILED\n";
        message.append(infoLog);
        Logger::Log("ShaderLinker", message, Logger::LOG_ERROR);
        return -1;
    }
    
    return programID;
}

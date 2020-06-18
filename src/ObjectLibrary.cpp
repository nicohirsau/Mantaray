#include "Mantaray/OpenGL/ObjectLibrary.hpp"
#include "Mantaray/OpenGL/Object.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/Objects/Texture.hpp"
#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"
#include "Mantaray/OpenGL/Objects/VertexArray.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

std::unordered_map<std::string, Object*> ObjectLibrary::Library = std::unordered_map<std::string, Object*>();
Logger ObjectLibrary::Logger("ObjectLibrary");

Shader* ObjectLibrary::CreateShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath) {
    Shader* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Shader(vertexShaderPath, fragmentShaderPath);
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

Shader* ObjectLibrary::CreateShader(std::string name, const char* vertexShaderSource, const char* fragmentShaderSource) {
    Shader* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Shader(vertexShaderSource, fragmentShaderSource);
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;    
}

Texture* ObjectLibrary::CreateTexture(std::string name, std::string imagePath) {
    Texture* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Texture(imagePath);
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

Texture* ObjectLibrary::CreateTexture(std::string name, Image &image) {
    Texture* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Texture(image);
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

Texture* ObjectLibrary::CreateTexture(std::string name, Vector2u resolution, int nrChannels) {
    Texture* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Texture(resolution, nrChannels);
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

RenderTexture* ObjectLibrary::CreateRenderTexture(std::string name, Vector2u resolution) {
    RenderTexture* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new RenderTexture(resolution);
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

RenderTexture* ObjectLibrary::CreateRenderTexture(std::string name, Vector2u resolution, Vector2f coordinateScale) {
    RenderTexture* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new RenderTexture(resolution, coordinateScale);
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

VertexArray* ObjectLibrary::CreateVertexArray(std::string name) {
    VertexArray* entry = nullptr;
    bool alreadyExistent = ObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        ObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new VertexArray();
        ObjectLibrary::Library[name] = entry;
        ObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

template<typename T>
bool ObjectLibrary::FindObject(std::string name, T*& outObject) {
    std::unordered_map<std::string, Object*>::const_iterator foundIterator = ObjectLibrary::Library.find(name);
    if (foundIterator == ObjectLibrary::Library.end()) {
        ObjectLibrary::Logger.Log("Object " + name + " could not be found in the library!", Logger::LOG_WARNING);
        return false;
    }
    T* foundEntry = dynamic_cast<T*>(foundIterator->second);
    if (foundEntry == nullptr) {
        ObjectLibrary::Logger.Log("Object " + name + " is not of the requested type!", Logger::LOG_WARNING);
        return false;
    }
    outObject = foundEntry;
    return true;
}

bool ObjectLibrary::DeleteObject(std::string name) {
    Object* entry;
    bool existsInLibray = ObjectLibrary::FindObject(name, entry);
    if (!existsInLibray) {
        ObjectLibrary::Logger.Log("Object " + name + " could not be found in the library!", Logger::LOG_WARNING);
        return false;
    }
    delete entry;
    entry = nullptr;
    ObjectLibrary::Library.erase(name);
    ObjectLibrary::Logger.Log("Object " + name + " has been removed from the library!", Logger::LOG_DEBUG);
    return true;
}

const char* defaultTexturedVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 textureCoordinate;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;
uniform vec4 u_textureSource;

out vec2 TexCoord;

void main(){
    gl_Position = u_projectionMatrix * u_modelMatrix * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
    TexCoord = vec2(
        textureCoordinate.x * u_textureSource.z + u_textureSource.x, 
        textureCoordinate.y * u_textureSource.w + u_textureSource.y
    );
}
)";

const char* defaultTexturedFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D u_texture0;
uniform vec4 u_color;

void main() {
    FragColor =  texture(u_texture0, TexCoord) * u_color;
}
)";

const char* defaultColoredVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 vertexPosition;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;

void main(){
    gl_Position = u_projectionMatrix * u_modelMatrix * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
}
)";

const char* defaultColoredFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 u_color;

void main() {
    FragColor = u_color;
}
)";

std::vector<Vector2f> defaultVertices = std::vector<Vector2f>({
    Vector2f(0, 0),
    Vector2f(1, 0),
    Vector2f(0, 1),
    Vector2f(1, 1)
});

std::vector<Vector2f> defaultTextureCoordinates = std::vector<Vector2f>({
    Vector2f(0, 0),
    Vector2f(1, 0),
    Vector2f(0, 1),
    Vector2f(1, 1)
});

std::vector<int> defaultIndices = std::vector<int>({
    0, 1, 2,
    2, 1, 3
});

VertexArray* ObjectLibrary::DefaultVertexArray = nullptr;
Shader* ObjectLibrary::DefaultTexturedShader = nullptr;
Shader* ObjectLibrary::DefaultColoredShader = nullptr;

void ObjectLibrary::InitializeDefaultEntries() {
    if (ObjectLibrary::DefaultVertexArray == nullptr) {
        ObjectLibrary::DefaultVertexArray = CreateVertexArray("DefaultVertexArray");
        ObjectLibrary::DefaultVertexArray->addVertices(defaultVertices);
        ObjectLibrary::DefaultVertexArray->addTextureCoordinates(defaultTextureCoordinates);
        ObjectLibrary::DefaultVertexArray->addIndices(defaultIndices);
        ObjectLibrary::DefaultVertexArray->uploadVertexArrayData();
    }
    if (ObjectLibrary::DefaultTexturedShader == nullptr) {
        ObjectLibrary::DefaultTexturedShader = CreateShader("DefaultTexturedShader", defaultTexturedVertexShaderSource, defaultTexturedFragmentShaderSource);
    }
    if (ObjectLibrary::DefaultColoredShader == nullptr) {
        ObjectLibrary::DefaultColoredShader = CreateShader("DefaultColoredShader", defaultColoredVertexShaderSource, defaultColoredFragmentShaderSource);
    }
}

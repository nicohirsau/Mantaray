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

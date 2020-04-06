#include "Mantaray/GLObjects/GLObjectLibrary.hpp"
#include "Mantaray/GLObjects/GLObject.hpp"
#include "Mantaray/GLObjects/Shader.hpp"
#include "Mantaray/GLObjects/Texture.hpp"
#include "Mantaray/GLObjects/RenderTexture.hpp"
#include "Mantaray/GLObjects/VertexArray.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

std::unordered_map<std::string, GLObject*> GLObjectLibrary::ObjectLibrary = std::unordered_map<std::string, GLObject*>();
Logger GLObjectLibrary::Logger("GLObjectLibrary");

Shader* GLObjectLibrary::CreateShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath) {
    Shader* entry = nullptr;
    bool alreadyExistent = GLObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        GLObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Shader(vertexShaderPath, fragmentShaderPath);
        GLObjectLibrary::ObjectLibrary[name] = entry;
        GLObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

Shader* GLObjectLibrary::CreateShader(std::string name, const char* vertexShaderSource, const char* fragmentShaderSource) {
    Shader* entry = nullptr;
    bool alreadyExistent = GLObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        GLObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Shader(vertexShaderSource, fragmentShaderSource);
        GLObjectLibrary::ObjectLibrary[name] = entry;
        GLObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;    
}

Texture* GLObjectLibrary::CreateTexture(std::string name, std::string imagePath) {
    Texture* entry = nullptr;
    bool alreadyExistent = GLObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        GLObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Texture(imagePath);
        GLObjectLibrary::ObjectLibrary[name] = entry;
        GLObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

Texture* GLObjectLibrary::CreateTexture(std::string name, Image &image) {
    Texture* entry = nullptr;
    bool alreadyExistent = GLObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        GLObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Texture(image);
        GLObjectLibrary::ObjectLibrary[name] = entry;
        GLObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

Texture* GLObjectLibrary::CreateTexture(std::string name, Vector2u resolution, int nrChannels) {
    Texture* entry = nullptr;
    bool alreadyExistent = GLObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        GLObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new Texture(resolution, nrChannels);
        GLObjectLibrary::ObjectLibrary[name] = entry;
        GLObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

RenderTexture* GLObjectLibrary::CreateRenderTexture(std::string name, unsigned int width, unsigned int height) {
    RenderTexture* entry = nullptr;
    bool alreadyExistent = GLObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        GLObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new RenderTexture(width, height);
        GLObjectLibrary::ObjectLibrary[name] = entry;
        GLObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

VertexArray* GLObjectLibrary::CreateVertexArray(std::string name) {
    VertexArray* entry = nullptr;
    bool alreadyExistent = GLObjectLibrary::FindObject(name, entry);
    if (alreadyExistent) {
        GLObjectLibrary::Logger.Log("Object " + name + " is already in library!", Logger::LOG_WARNING);
    }
    else {
        entry = new VertexArray();
        GLObjectLibrary::ObjectLibrary[name] = entry;
        GLObjectLibrary::Logger.Log("Object " + name + " has been added to the library!", Logger::LOG_DEBUG);
    }
    return entry;
}

template<typename T>
bool GLObjectLibrary::FindObject(std::string name, T*& outObject) {
    std::unordered_map<std::string, GLObject*>::const_iterator foundIterator = GLObjectLibrary::ObjectLibrary.find(name);
    if (foundIterator == GLObjectLibrary::ObjectLibrary.end()) {
        GLObjectLibrary::Logger.Log("Object " + name + " could not be found in the library!", Logger::LOG_WARNING);
        return false;
    }
    T* foundEntry = dynamic_cast<T*>(foundIterator->second);
    if (foundEntry == nullptr) {
        GLObjectLibrary::Logger.Log("Object " + name + " is not of the requestet type!", Logger::LOG_WARNING);
        return false;
    }
    outObject = foundEntry;
    return true;
}

bool GLObjectLibrary::DeleteObject(std::string name) {
    GLObject* entry;
    bool existsInLibray = GLObjectLibrary::FindObject(name, entry);
    if (!existsInLibray) {
        GLObjectLibrary::Logger.Log("Object " + name + " could not be found in the library!", Logger::LOG_WARNING);
        return false;
    }
    delete entry;
    entry = nullptr;
    GLObjectLibrary::ObjectLibrary.erase(name);
    GLObjectLibrary::Logger.Log("Object " + name + " has been removed from the library!", Logger::LOG_DEBUG);
    return true;
}

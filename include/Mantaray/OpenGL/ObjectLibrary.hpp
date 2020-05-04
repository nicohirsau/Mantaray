#pragma once

#include <unordered_map>
#include <string>

#include "Mantaray/Core/Vector.hpp"

namespace MR {
class ObjectLibrary {
    public:
        static class Shader* CreateShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath);
        static class Shader* CreateShader(std::string name, const char* vertexShaderSource, const char* fragmentShaderSource);
        static class Texture* CreateTexture(std::string name, std::string imagePath);
        static class Texture* CreateTexture(std::string name, class Image &image);
        static class Texture* CreateTexture(std::string name, Vector2u resolution, int nrChannels = 4);
        static class RenderTexture* CreateRenderTexture(std::string name, unsigned int width, unsigned int height);
        static class VertexArray* CreateVertexArray(std::string name);
        
        template<typename T>
        static bool FindObject(std::string name, T*& outObject);
        static bool DeleteObject(std::string name);

    private:
        static std::unordered_map<std::string, class Object*> Library;
        static class Logger Logger;
};
}
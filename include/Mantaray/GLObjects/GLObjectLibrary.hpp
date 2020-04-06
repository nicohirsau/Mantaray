#pragma once

#include <unordered_map>
#include <string>

namespace MR {
class GLObjectLibrary {
    public:
        static class Shader* CreateShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath);
        static class Texture* CreateTexture(std::string name, std::string imagePath);
        static class RenderTexture* CreateRenderTexture(std::string name, unsigned int width, unsigned int height);
        static class VertexArray* CreateVertexArray(std::string name);
        
        template<typename T>
        static bool FindObject(std::string name, T*& outObject);
        static bool DeleteObject(std::string name);

    private:
        static std::unordered_map<std::string, class GLObject*> ObjectLibrary;
        static class Logger Logger;
};
}

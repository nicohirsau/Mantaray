#pragma once

#include <string>

#include "Mantaray/Core/Vector.hpp"

class GLFWwindow;

namespace MR {
class Context {
    public:
        static bool Create(GLFWwindow** outWindow, std::string title, Vector2u size);
        static void Destroy();
    
    private:
        static bool IsInitialized;
};
}
#pragma once

#include <string>

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/Core/Shapes.hpp"
#include "Mantaray/Core/Timer.hpp"
#include "Mantaray/OpenGL/Drawables.hpp"

namespace MR {
class Window {
    public:
        Window(std::string title, Vector2u size, bool shouldKeepAspectRatio = true);
        Window(std::string title, Vector2u size, Vector2u resolution, bool shouldKeepAspectRatio = true);
        Window(std::string title, Vector2u size, Vector2u resolution, Vector2f coordinateScale, bool shouldKeepAspectRatio = true);
        ~Window();

        bool shouldClose();
        float update();
        void beginFrame();
        void endFrame();

        void draw(Sprite& sprite);
        void draw(Polygon& polygon);

    protected:
        static void OnWindowResized(class GLFWwindow* window, int width, int height);
        void calculateViewDestination(int windowWidth, int windowHeight);
    
    private:
        static Window* Instance;
        class GLFWwindow* m_Window = nullptr;
        class Canvas* m_Canvas = nullptr;
        Rectanglei m_ViewPort;
        Rectanglef m_ViewDestination;
        Timer m_Timer;
        bool m_ShouldKeepAspectRatio;
        float m_PrefferedAspectRatio;
};
}
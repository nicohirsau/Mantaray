#include "Mantaray/OpenGL/Objects/Canvas.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/ObjectLibrary.hpp"
#include "Mantaray/Core/InputManager.hpp"
#include "Mantaray/Core/Window.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

Canvas::Canvas(Vector2u resolution) : RenderTexture(resolution) {
    setDisplaySpace(m_DisplaySpace);
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Canvas::Canvas(Vector2u resolution, Rectanglef displaySpace) : RenderTexture(resolution) {
    setDisplaySpace(m_DisplaySpace);
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Canvas::Canvas(Vector2u resolution, Vector2f coordinateScale) : RenderTexture(resolution, coordinateScale) {
    setDisplaySpace(m_DisplaySpace);
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Canvas::Canvas(Vector2u resolution, Vector2f coordinateScale, Rectanglef displaySpace) : RenderTexture(resolution, coordinateScale) {
    setDisplaySpace(displaySpace);
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Rectanglef Canvas::getDisplaySpace() {
    return m_DisplaySpace;
}

void Canvas::setDisplaySpace(Rectanglef displaySpace) {
    Window* windowInstance = Window::GetInstance();
    if (windowInstance == nullptr) {
        Logger::Log("View", "Cant set displayspace, Window is nullptr!", Logger::LOG_WARNING);
        return;
    }

    m_DisplaySpace = Rectanglef(
        displaySpace.x() * windowInstance->getSize().x,
        displaySpace.y() * windowInstance->getSize().y,
        displaySpace.width() * windowInstance->getSize().x,
        displaySpace.height() * windowInstance->getSize().y
    );
}

Vector2d Canvas::getMousePosition() {
    Vector2d windowMousePosition = InputManager::GetMousePosition();
    // idk atm

    return Vector2d();
}

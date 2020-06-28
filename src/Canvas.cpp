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
    setDisplaySpace(displaySpace);
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

#include <iostream>

void Canvas::setDisplaySpace(Rectanglef displaySpace) {
    Window* windowInstance = Window::GetInstance();

    std::cout << "Value of windowpointer: " << windowInstance << std::endl;
    if (windowInstance == nullptr) {
        m_DisplaySpace = Rectanglef(
            displaySpace.x() * m_CoordinateScale.x,
            displaySpace.y() * m_CoordinateScale.y,
            displaySpace.width() * m_CoordinateScale.x,
            displaySpace.height() * m_CoordinateScale.y
        );
        return;
    }

    m_DisplaySpace = Rectanglef(
        displaySpace.x() * windowInstance->getCoordinateScale().x,
        displaySpace.y() * windowInstance->getCoordinateScale().y,
        displaySpace.width() * windowInstance->getCoordinateScale().x,
        displaySpace.height() * windowInstance->getCoordinateScale().y
    );
}

Vector2d Canvas::getMousePosition() {
    Vector2d windowMousePosition = InputManager::GetMousePosition();
    // idk atm

    return Vector2d();
}

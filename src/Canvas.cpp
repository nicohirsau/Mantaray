#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "Mantaray/OpenGL/Objects/Canvas.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/ObjectLibrary.hpp"
#include "Mantaray/Core/InputManager.hpp"
#include "Mantaray/Core/Window.hpp"
#include "Mantaray/Core/Logger.hpp"

#include <iostream>

using namespace MR;

Canvas::Canvas(Vector2u resolution) : RenderTexture(resolution) {
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Canvas::Canvas(Vector2u resolution, Rectanglef displaySpace) : RenderTexture(resolution) {
    m_DisplaySpace = displaySpace;
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Canvas::Canvas(Vector2u resolution, Vector2f coordinateScale) : RenderTexture(resolution, coordinateScale) {
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Canvas::Canvas(Vector2u resolution, Vector2f coordinateScale, Rectanglef displaySpace) : RenderTexture(resolution, coordinateScale) {
    m_DisplaySpace = displaySpace;
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
}

Rectanglef Canvas::getDisplaySpace() {
    return m_DisplaySpace;
}

Vector2f Canvas::getMousePosition() {
    Window* windowInstance = Window::GetInstance();
    if (windowInstance == nullptr) {
        return Vector2f(0, 0);
    }

    Vector2d windowMousePosition = InputManager::GetMousePosition();
    windowMousePosition.y = Window::GetInstance()->getSize().y - windowMousePosition.y;

    glm::mat4 projection = createProjectionMatrix();

    glm::vec4 viewport = glm::vec4(
        windowInstance->getViewportRect().x() + m_DisplaySpace.x() * windowInstance->getViewportRect().width(), 
        windowInstance->getViewportRect().y() +  m_DisplaySpace.y() * windowInstance->getViewportRect().height(), 
        m_DisplaySpace.width() * windowInstance->getViewportRect().width(), 
        m_DisplaySpace.height() * windowInstance->getViewportRect().height()
    );

    glm::vec3 worldCoordinate = glm::unProject(glm::vec3(windowMousePosition.x, windowMousePosition.y, 0), glm::mat4(1.f), projection, viewport);
    return Vector2f(worldCoordinate.x, worldCoordinate.y);
}

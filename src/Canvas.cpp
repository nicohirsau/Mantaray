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

Vector2f Canvas::getMousePosition() {
    Vector2d windowMousePosition = InputManager::GetMousePosition();
    windowMousePosition.y = Window::GetInstance()->getSize().y - windowMousePosition.y;
    glm::mat4 projection = createProjectionMatrix();
    glm::vec4 viewport = glm::vec4(m_DisplaySpace.x(), m_DisplaySpace.y(), m_DisplaySpace.width(), m_DisplaySpace.height());
    glm::vec3 worldCoordinate = glm::unProject(glm::vec3(windowMousePosition.x, windowMousePosition.y, 0), glm::mat4(1.f), projection, viewport);
    return Vector2f(worldCoordinate.x, worldCoordinate.y);
}

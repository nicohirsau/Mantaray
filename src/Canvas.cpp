#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "Mantaray/OpenGL/Objects/Canvas.hpp"
#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"
#include "Mantaray/OpenGL/Objects/Texture.hpp"
#include "Mantaray/OpenGL/Objects/VertexArray.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/Drawables.hpp"
#include "Mantaray/Core/Window.hpp"

using namespace MR;

const char* defaultTexturedVertexShaderSource2 = R"(
#version 330 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 textureCoordinate;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;
uniform vec4 u_textureSource;

out vec2 TexCoord;

void main(){
    gl_Position = u_projectionMatrix * u_modelMatrix * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
    TexCoord = vec2(
        textureCoordinate.x * u_textureSource.z + u_textureSource.x, 
        textureCoordinate.y * u_textureSource.w + u_textureSource.y
    );
}
)";

const char* defaultTexturedFragmentShaderSource2 = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D u_texture0;
uniform vec4 u_color;

void main() {
    FragColor =  texture(u_texture0, TexCoord) * u_color;
}
)";

Canvas::Canvas(Vector2u resolution) : RenderTexture(resolution) {
    m_Shader = new Shader(defaultTexturedVertexShaderSource2, defaultTexturedFragmentShaderSource2);

}

Canvas::Canvas(Vector2u resolution, Vector2f coordinateScale) : RenderTexture(resolution, coordinateScale){
    m_Shader = new Shader(defaultTexturedVertexShaderSource2, defaultTexturedFragmentShaderSource2);
}

Canvas::~Canvas() {
    delete m_Shader;
    m_Shader = nullptr;
}

Shader* Canvas::getShader() {
    return m_Shader;
}

void Canvas::setShader(Shader* shader) {
    delete m_Shader;
    m_Shader = shader;
}

Rectanglef Canvas::getViewPortDestination() {
    return m_ViewPortDestination;
}

void Canvas::setViewPortDestination(Rectanglef viewPortDestination) {
    m_ViewPortDestination = viewPortDestination;
}

void Canvas::display() {
    Vector2i windowSize = Window::GetInstance()->getSize();
    Rectangleu absoluteViewPortDestination = Rectangleu(
        m_ViewPortDestination.x() * windowSize.x,
        m_ViewPortDestination.y() * windowSize.y,
        m_ViewPortDestination.width() * windowSize.x,
        m_ViewPortDestination.height() * windowSize.y
    );
    
    unbind();
    glViewport(
        absoluteViewPortDestination.x(), absoluteViewPortDestination.y(), 
        absoluteViewPortDestination.width(), absoluteViewPortDestination.height()
    );
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 projection = glm::ortho(
        static_cast<float>(absoluteViewPortDestination.x()),
        static_cast<float>(absoluteViewPortDestination.x() + absoluteViewPortDestination.width()),
        static_cast<float>(absoluteViewPortDestination.y()),
        static_cast<float>(absoluteViewPortDestination.y() + absoluteViewPortDestination.height()),
        -1.0f, 1.0f
    );
    m_Shader->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(absoluteViewPortDestination.x(), absoluteViewPortDestination.y(), 0.0f));

    model = glm::scale(model, glm::vec3(absoluteViewPortDestination.width(), absoluteViewPortDestination.height(), 1.0f));

    m_Shader->setUniformMatrix4("u_modelMatrix", model);
    m_Shader->setUniformVector4f("u_textureSource", Vector4f(0, 0, 1, 1));
    m_Shader->setRenderTexture("u_texture0", 0, *this);
    m_Shader->setUniformVector4f("u_color", Vector4f(1, 1, 1, 1));
    m_Shader->setupForDraw();
    RenderTexture::DefaultVertexArray->draw();
}


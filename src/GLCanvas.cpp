#include <vector>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "Mantaray/GLObjects/GLCanvas.hpp"
#include "Mantaray/GLObjects/RenderTexture.hpp"
#include "Mantaray/GLObjects/Texture.hpp"
#include "Mantaray/GLObjects/VertexArray.hpp"
#include "Mantaray/GLObjects/Shader.hpp"

using namespace MR;

const char* defaultVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 textureCoordinate;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;
uniform vec4 u_textureSource;

out vec2 TexCoord;

void main(){
    gl_Position = u_projectionMatrix * u_modelMatrix * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
    TexCoord = textureCoordinate;
    //vec2(
    //    textureCoordinate.x * u_textureSource.z + u_textureSource.x, 
    //    textureCoordinate.y * u_textureSource.w + u_textureSource.y
    //);
}
)";

const char* defaultFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D u_texture0;

void main() {
    FragColor =  texture(u_texture0, TexCoord);
}
)";

std::vector<Vector2f> defaultVertices = std::vector<Vector2f>({
    Vector2f(0, 0),
    Vector2f(1, 0),
    Vector2f(0, 1),
    Vector2f(1, 1)
});

std::vector<Vector2f> defaultTextureCoordinates = std::vector<Vector2f>({
    Vector2f(0, 0),
    Vector2f(1, 0),
    Vector2f(0, 1),
    Vector2f(1, 1)
});

std::vector<int> defaultIndices = std::vector<int>({
    0, 1, 2,
    2, 1, 3
});

VertexArray* GLCanvas::DefaultVertexArray = nullptr;
Shader* GLCanvas::DefaultShader = nullptr;


GLCanvas::GLCanvas(Vector2u resolution, Vector2f coordinateScale) {
    init(resolution, coordinateScale);
}

GLCanvas::GLCanvas(Vector2u resolution) {
    init(resolution, Vector2f(resolution.x, resolution.y));
}

GLCanvas::~GLCanvas() {
    delete m_RenderTexture;
    delete m_Shader;
}

void GLCanvas::init(Vector2u resolution, Vector2f coordinateScale) {
    m_RenderTexture = new RenderTexture(resolution.x, resolution.y);
    m_CoordinateScale = Vector2f(coordinateScale.x, coordinateScale.y);

    m_Shader = new Shader(defaultVertexShaderSource, defaultFragmentShaderSource);
    m_Shader->setRenderTexture("u_texture0", 0, *m_RenderTexture);

    if (GLCanvas::DefaultVertexArray == nullptr) {
        GLCanvas::DefaultVertexArray = new VertexArray();
        GLCanvas::DefaultVertexArray->addVertices(defaultVertices);
        GLCanvas::DefaultVertexArray->addTextureCoordinates(defaultTextureCoordinates);
        GLCanvas::DefaultVertexArray->addIndices(defaultIndices);
        GLCanvas::DefaultVertexArray->uploadVertexArrayData();
    }
    if (GLCanvas::DefaultShader == nullptr) {
        GLCanvas::DefaultShader = new Shader(defaultVertexShaderSource, defaultFragmentShaderSource);
    }
}

Shader* GLCanvas::getShader() {
    return m_Shader;
}

void GLCanvas::setShader(Shader* shader) {
    delete m_Shader;
    m_Shader = shader;
}

RenderTexture* GLCanvas::getRenderTexture() {
    return m_RenderTexture;
}

void GLCanvas::bind() {
    m_RenderTexture->bind();
    glViewport(0, 0, m_RenderTexture->getWidth(), m_RenderTexture->getHeight());
}

void GLCanvas::unbind() {
    m_RenderTexture->unbind();
}

void GLCanvas::clear(Color color) {
    glClearColor(
        color.r / 255.f,
        color.g / 255.f,
        color.b / 255.f,
        color.a / 255.f
    );
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLCanvas::setOffset(Vector2f offset) {
    m_Offset = offset;
}

void GLCanvas::addOffset(Vector2f offset) {
    m_Offset = m_Offset + offset;
}

void GLCanvas::display(Rectanglei viewPort) {
    display(viewPort, Rectanglef(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height()));
}

void GLCanvas::display(Rectanglei viewPort, Rectanglef destination) {
    unbind();
    glViewport(viewPort.x(), viewPort.y(), (unsigned int)viewPort.width(), (unsigned int)viewPort.height());
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 projection = glm::ortho(
        static_cast<float>(viewPort.x()),
        static_cast<float>(viewPort.x() + viewPort.width()),
        static_cast<float>(viewPort.y()),
        static_cast<float>(viewPort.y() + viewPort.height()),
        -1.0f, 1.0f
    );
    m_Shader->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(destination.x(), destination.y(), 0.0f));
    model = glm::scale(model, glm::vec3(destination.width(), destination.height(), 1.0f));
    m_Shader->setUniformMatrix4("u_modelMatrix", model);
    m_Shader->setUniformVector4f("u_textureSource", Vector4f(0, 0, 1, 1));
    m_Shader->setRenderTexture("u_texture0", 0, *m_RenderTexture);
    m_Shader->setupForDraw();
    GLCanvas::DefaultVertexArray->draw();
}

void GLCanvas::draw(
    Texture* texture, 
    Vector2f position,
    Vector2f size,
    bool absoluteSize,
    float rotation,
    Vector2f rotationCenter,
    Rectanglef sourceRectangle,
    Color color,
    Shader* shader
    ) {
    Shader* shaderToUse = shader;
    if (shaderToUse == nullptr) {
        shaderToUse = GLCanvas::DefaultShader;
    }
    shaderToUse->setTexture("u_texture0", 0, *texture);
    
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_CoordinateScale.x), 0.0f, static_cast<float>(m_CoordinateScale.y), -1.0f, 1.0f);
    projection = glm::translate(projection, glm::vec3(-m_Offset.x, -m_Offset.y, 0));
    shaderToUse->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::translate(model, -glm::vec3(rotationCenter.x, rotationCenter.y, 0.0f));
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(rotationCenter.x, rotationCenter.y, 0.0f));
    
    if (absoluteSize) {
        model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
    }
    else {
        model = glm::scale(model, glm::vec3(size.x * texture->getWidth(), size.y * texture->getHeight(), 1.0f));
    }
    shaderToUse->setUniformMatrix4("u_modelMatrix", model);
    shaderToUse->setUniformVector4f("u_textureSource", Vector4f(sourceRectangle.x(), sourceRectangle.y(), sourceRectangle.width(), sourceRectangle.height()));
    shaderToUse->setupForDraw();
    GLCanvas::DefaultVertexArray->draw();
}

#include <vector>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "Mantaray/OpenGL/Canvas.hpp"
#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"
#include "Mantaray/OpenGL/Objects/Texture.hpp"
#include "Mantaray/OpenGL/Objects/VertexArray.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/Drawables.hpp"

using namespace MR;

const char* defaultTexturedVertexShaderSource = R"(
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

const char* defaultTexturedFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D u_texture0;
uniform vec4 u_color;

void main() {
    FragColor =  texture(u_texture0, TexCoord) * u_color;
}
)";

const char* defaultColoredVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 vertexPosition;

uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;

void main(){
    gl_Position = u_projectionMatrix * u_modelMatrix * vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0);
}
)";

const char* defaultColoredFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 u_color;

void main() {
    FragColor = u_color;
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

VertexArray* Canvas::DefaultVertexArray = nullptr;
Shader* Canvas::DefaultTexturedShader = nullptr;
Shader* Canvas::DefaultColoredShader = nullptr;


Canvas::Canvas(Vector2u resolution, Vector2f coordinateScale) {
    init(resolution, coordinateScale);
}

Canvas::Canvas(Vector2u resolution) {
    init(resolution, Vector2f(resolution.x, resolution.y));
}

Canvas::~Canvas() {
    delete m_RenderTexture;
    delete m_Shader;
}

void Canvas::init(Vector2u resolution, Vector2f coordinateScale) {
    m_RenderTexture = new RenderTexture(resolution.x, resolution.y);
    m_CoordinateScale = Vector2f(coordinateScale.x, coordinateScale.y);

    m_Shader = new Shader(defaultTexturedVertexShaderSource, defaultTexturedFragmentShaderSource);
    m_Shader->setRenderTexture("u_texture0", 0, *m_RenderTexture);

    if (Canvas::DefaultVertexArray == nullptr) {
        Canvas::DefaultVertexArray = new VertexArray();
        Canvas::DefaultVertexArray->addVertices(defaultVertices);
        Canvas::DefaultVertexArray->addTextureCoordinates(defaultTextureCoordinates);
        Canvas::DefaultVertexArray->addIndices(defaultIndices);
        Canvas::DefaultVertexArray->uploadVertexArrayData();
    }
    if (Canvas::DefaultTexturedShader == nullptr) {
        Canvas::DefaultTexturedShader = new Shader(defaultTexturedVertexShaderSource, defaultTexturedFragmentShaderSource);
    }
    if (Canvas::DefaultColoredShader == nullptr) {
        Canvas::DefaultColoredShader = new Shader(defaultColoredVertexShaderSource, defaultColoredFragmentShaderSource);
    }
}

Shader* Canvas::getShader() {
    return m_Shader;
}

void Canvas::setShader(Shader* shader) {
    delete m_Shader;
    m_Shader = shader;
}

RenderTexture* Canvas::getRenderTexture() {
    return m_RenderTexture;
}

void Canvas::setCoordinateScale(Vector2f coordinateScale) {
    m_CoordinateScale = coordinateScale;
}

Vector2f Canvas::getCoordinateScale() {
    return m_CoordinateScale;
}

void Canvas::setOffset(Vector2f offset) {
    m_Offset = offset;
}

void Canvas::addOffset(Vector2f offset) {
    m_Offset = m_Offset + offset;
}

Vector2f Canvas::getOffset() {
    return m_Offset;
}

void Canvas::bind() {
    m_RenderTexture->bind();
    glViewport(0, 0, m_RenderTexture->getWidth(), m_RenderTexture->getHeight());
}

void Canvas::bind(Color color) {
    bind();
    clear(color);
}

void Canvas::unbind() {
    m_RenderTexture->unbind();
}

void Canvas::clear(Color color) {
    glClearColor(
        color.r / 255.f,
        color.g / 255.f,
        color.b / 255.f,
        color.a / 255.f
    );
    glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas::display(Rectanglei viewPort) {
    display(viewPort, Rectanglef(viewPort.x(), viewPort.y(), viewPort.width(), viewPort.height()));
}

void Canvas::display(Rectanglei viewPort, Rectanglef destination) {
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
    m_Shader->setUniformVector4f("u_color", Vector4f(1, 1, 1, 1));
    m_Shader->setupForDraw();
    Canvas::DefaultVertexArray->draw();
}

void Canvas::draw(Sprite& sprite) {
    draw(
        sprite.texture,
        sprite.position,
        sprite.size,
        sprite.absoluteSize,
        sprite.rotation,
        sprite.rotationCenter,
        sprite.sourceRectangle,
        sprite.color,
        sprite.shader
    );
}

void Canvas::draw(
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
    if (texture == nullptr) {
        return;
    }

    Shader* shaderToUse = shader;
    if (shaderToUse == nullptr) {
        shaderToUse = Canvas::DefaultTexturedShader;
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
        model = glm::scale(
            model, 
            glm::vec3(
                size.x * texture->getWidth() * sourceRectangle.width(), 
                size.y * texture->getHeight() * sourceRectangle.height(), 
                1.0f
            )
        );
    }
    shaderToUse->setUniformMatrix4("u_modelMatrix", model);
    shaderToUse->setUniformVector4f("u_textureSource", Vector4f(sourceRectangle.x(), sourceRectangle.y(), sourceRectangle.width(), sourceRectangle.height()));
    shaderToUse->setUniformVector4f("u_color", Vector4f(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
    shaderToUse->setupForDraw();
    Canvas::DefaultVertexArray->draw();
}

void Canvas::draw(Polygon& polygon) {
    draw(
        polygon.vertexArray,
        polygon.position,
        polygon.size,
        polygon.absoluteSize,
        polygon.rotation,
        polygon.rotationCenter,
        polygon.color,
        polygon.shader,
        polygon.texture,
        polygon.sourceRectangle
    );
}

void Canvas::draw(
        class VertexArray* vertexArray,
        Vector2f position,
        Vector2f size,
        bool absoluteSize,
        float rotation,
        Vector2f rotationCenter,
        Color color,
        class Shader* shader,
        class Texture* texture,
        Rectanglef sourceRectangle
    ) {
    if (vertexArray == nullptr) {
        return;
    }

    Shader* shaderToUse = shader;
    if (shaderToUse == nullptr) {
        if (texture != nullptr) {
            shaderToUse = Canvas::DefaultTexturedShader;
            shaderToUse->setTexture("u_texture0", 0, *texture);
            shaderToUse->setUniformVector4f(
                "u_textureSource", 
                Vector4f(sourceRectangle.x(), sourceRectangle.y(), sourceRectangle.width(), sourceRectangle.height())
            );
        }
        if (texture == nullptr) {
            shaderToUse = Canvas::DefaultColoredShader;
        }
    }
    
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
        model = glm::scale(
            model, 
            glm::vec3(
                size.x * texture->getWidth() * sourceRectangle.width(), 
                size.y * texture->getHeight() * sourceRectangle.height(), 
                1.0f
            )
        );
    }
    shaderToUse->setUniformMatrix4("u_modelMatrix", model);
    shaderToUse->setUniformVector4f("u_color", Vector4f(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
    shaderToUse->setupForDraw();
    vertexArray->draw();    
}

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <vector>

#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/Objects/VertexArray.hpp"
#include "Mantaray/Core/Logger.hpp"
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

VertexArray* RenderTexture::DefaultVertexArray = nullptr;
Shader* RenderTexture::DefaultTexturedShader = nullptr;
Shader* RenderTexture::DefaultColoredShader = nullptr;

RenderTexture::RenderTexture(Vector2u resolution) {
    link();
    init(resolution, Vector2f(resolution.x, resolution.y));
}

RenderTexture::RenderTexture(Vector2u resolution, Vector2f coordinateScale) {
    link();
    init(resolution, coordinateScale);
}

RenderTexture::~RenderTexture() {
    unlink();
    delete m_RenderTexture;
    m_RenderTexture = nullptr;
}

void RenderTexture::init(Vector2u resolution, Vector2f coordinateScale) {
    m_Resolution = resolution;
    m_CoordinateScale = coordinateScale;

    if (RenderTexture::DefaultVertexArray == nullptr) {
        RenderTexture::DefaultVertexArray = new VertexArray();
        RenderTexture::DefaultVertexArray->addVertices(defaultVertices);
        RenderTexture::DefaultVertexArray->addTextureCoordinates(defaultTextureCoordinates);
        RenderTexture::DefaultVertexArray->addIndices(defaultIndices);
        RenderTexture::DefaultVertexArray->uploadVertexArrayData();
    }
    if (RenderTexture::DefaultTexturedShader == nullptr) {
        RenderTexture::DefaultTexturedShader = new Shader(defaultTexturedVertexShaderSource, defaultTexturedFragmentShaderSource);
    }
    if (RenderTexture::DefaultColoredShader == nullptr) {
        RenderTexture::DefaultColoredShader = new Shader(defaultColoredVertexShaderSource, defaultColoredFragmentShaderSource);
    }
}

void RenderTexture::allocate() {
    glGenFramebuffers(1, &m_FBO);
    bind();
    m_RenderTexture = new Texture(m_Resolution);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexture->m_TextureID, 0);
    unbind();
}

void RenderTexture::release() {
    glDeleteFramebuffers(1, &m_FBO);
}

void RenderTexture::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glViewport(0, 0, m_Resolution.x, m_Resolution.y);
}

void RenderTexture::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::clear(Color color) {
    bind();
    glClearColor(
        color.r / 255.f,
        color.g / 255.f,
        color.b / 255.f,
        color.a / 255.f
    );
    glClear(GL_COLOR_BUFFER_BIT);
}

int RenderTexture::getWidth() {
    return m_Resolution.x;
}

int RenderTexture::getHeight() {
    return m_Resolution.y;
}

Vector2f RenderTexture::getCoordinateScale() {
    return m_CoordinateScale;
}

void RenderTexture::setCoordinateScale(Vector2f coordinateScale) {
    m_CoordinateScale = coordinateScale;
}

Vector2f RenderTexture::getOffset() {
    return m_Offset;
}

void RenderTexture::setOffset(Vector2f offset) {
    m_Offset = offset;
}

void RenderTexture::addOffset(Vector2f offset) {
    m_Offset = m_Offset + offset;
}

float RenderTexture::getScale() {
    return m_Scale;
}

void RenderTexture::setScale(float scale) {
    m_Scale = scale;
}

Vector2f RenderTexture::getScaleCenter() {
    return m_ScaleCenter;
}

void RenderTexture::setScaleCenter(Vector2f scaleCenter) {
    m_ScaleCenter = scaleCenter;
}

//void RenderTexture::activate() {
//    bind();
//}

//void RenderTexture::activate(Color color) {
//    bind();
//    clear(color);
//}
//
//void RenderTexture::deactivate() {
//    unbind();
//}

void RenderTexture::draw(Sprite& sprite) {
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

void RenderTexture::draw(
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
        shaderToUse = RenderTexture::DefaultTexturedShader;
    }
    shaderToUse->setTexture("u_texture0", 0, *texture);
    
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_CoordinateScale.x), 0.0f, static_cast<float>(m_CoordinateScale.y), -1.0f, 1.0f);

    projection = glm::translate(projection, glm::vec3(m_ScaleCenter.x * m_CoordinateScale.x, m_ScaleCenter.y * m_CoordinateScale.y, 0.0f));
    projection = glm::scale(projection, glm::vec3(m_Scale));
    projection = glm::translate(projection, -glm::vec3(m_ScaleCenter.x * m_CoordinateScale.x, m_ScaleCenter.y * m_CoordinateScale.y, 0.0f));

    projection = glm::translate(projection, glm::vec3(-m_Offset.x, -m_Offset.y, 0));
    shaderToUse->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    
    if (absoluteSize) {
        model = glm::translate(model, glm::vec3(rotationCenter.x * size.x, rotationCenter.y * size.y, 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, -glm::vec3(rotationCenter.x * size.x, rotationCenter.y * size.y, 0.0f));

        model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
    }
    else {
        model = glm::translate(model, glm::vec3(rotationCenter.x * size.x * texture->getWidth() * sourceRectangle.width(), rotationCenter.y * size.y * texture->getHeight() * sourceRectangle.height(), 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, -glm::vec3(rotationCenter.x * size.x * texture->getWidth() * sourceRectangle.width(), rotationCenter.y * size.y * texture->getHeight() * sourceRectangle.height(), 0.0f));

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
    RenderTexture::DefaultVertexArray->draw();
}

void RenderTexture::draw(Polygon& polygon) {
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

void RenderTexture::draw(
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
            shaderToUse = RenderTexture::DefaultTexturedShader;
            shaderToUse->setTexture("u_texture0", 0, *texture);
            shaderToUse->setUniformVector4f(
                "u_textureSource", 
                Vector4f(sourceRectangle.x(), sourceRectangle.y(), sourceRectangle.width(), sourceRectangle.height())
            );
        }
        if (texture == nullptr) {
            shaderToUse = RenderTexture::DefaultColoredShader;
        }
    }
    
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_CoordinateScale.x), 0.0f, static_cast<float>(m_CoordinateScale.y), -1.0f, 1.0f);

    projection = glm::translate(projection, glm::vec3(m_ScaleCenter.x * m_CoordinateScale.x, m_ScaleCenter.y * m_CoordinateScale.y, 0.0f));
    projection = glm::scale(projection, glm::vec3(m_Scale));
    projection = glm::translate(projection, -glm::vec3(m_ScaleCenter.x * m_CoordinateScale.x, m_ScaleCenter.y * m_CoordinateScale.y, 0.0f));

    projection = glm::translate(projection, glm::vec3(-m_Offset.x, -m_Offset.y, 0));
    shaderToUse->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    
    if (absoluteSize) {
        model = glm::translate(model, glm::vec3(rotationCenter.x * size.x, rotationCenter.y * size.y, 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, -glm::vec3(rotationCenter.x * size.x, rotationCenter.y * size.y, 0.0f));

        model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
    }
    else {
        model = glm::translate(model, glm::vec3(rotationCenter.x * size.x * texture->getWidth() * sourceRectangle.width(), rotationCenter.y * size.y * texture->getHeight() * sourceRectangle.height(), 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, -glm::vec3(rotationCenter.x * size.x * texture->getWidth() * sourceRectangle.width(), rotationCenter.y * size.y * texture->getHeight() * sourceRectangle.height(), 0.0f));

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

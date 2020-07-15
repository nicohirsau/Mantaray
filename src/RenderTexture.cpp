#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <vector>

#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"
#include "Mantaray/OpenGL/Objects/Shader.hpp"
#include "Mantaray/OpenGL/Objects/VertexArray.hpp"
#include "Mantaray/OpenGL/Objects/Canvas.hpp"
#include "Mantaray/Core/Logger.hpp"
#include "Mantaray/OpenGL/Drawables.hpp"
#include "Mantaray/OpenGL/ObjectLibrary.hpp"
#include "Mantaray/Core/Window.hpp"

using namespace MR;

VertexArray* RenderTexture::DefaultVertexArray = nullptr;
Shader* RenderTexture::DefaultTexturedShader = nullptr;
Shader* RenderTexture::DefaultColoredShader = nullptr;

RenderTexture::RenderTexture(Vector2u resolution) {
    m_Resolution = resolution;
    m_CoordinateScale = Vector2f(resolution.x, resolution.y);
    setDefaults();
    link();
}

RenderTexture::RenderTexture(Vector2u resolution, Vector2f coordinateScale) {
    m_Resolution = resolution;
    m_CoordinateScale = coordinateScale;
    setDefaults();
    link();
}

RenderTexture::~RenderTexture() {
    unlink();
    delete m_RenderTexture;
    m_RenderTexture = nullptr;
}

void RenderTexture::setDefaults() {
    if (RenderTexture::DefaultVertexArray == nullptr) {
        ObjectLibrary::FindObject("DefaultVertexArray", RenderTexture::DefaultVertexArray);
    }
    if (RenderTexture::DefaultTexturedShader == nullptr) {
        ObjectLibrary::FindObject("DefaultTexturedShader", RenderTexture::DefaultTexturedShader);
    }
    if (RenderTexture::DefaultColoredShader == nullptr) {
        ObjectLibrary::FindObject("DefaultColoredShader", RenderTexture::DefaultColoredShader);
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

glm::mat4 RenderTexture::createProjectionMatrix(bool scaled, bool shifted) {
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_CoordinateScale.x), 0.0f, static_cast<float>(m_CoordinateScale.y), -1.0f, 1.0f);

    if (scaled && m_Scale != 0.f) {
        projection = glm::translate(projection, glm::vec3(m_ScaleCenter.x * m_CoordinateScale.x, m_ScaleCenter.y * m_CoordinateScale.y, 0.0f));
        projection = glm::scale(projection, glm::vec3(m_Scale));
        projection = glm::translate(projection, -glm::vec3(m_ScaleCenter.x * m_CoordinateScale.x, m_ScaleCenter.y * m_CoordinateScale.y, 0.0f));
    }
    if (shifted && m_Offset != Vector2f(0, 0)) {
        projection = glm::translate(projection, glm::vec3(-m_Offset.x, -m_Offset.y, 0));
    }

    return projection;
}

glm::mat4 RenderTexture::createModelMatrix(Vector2f position, Vector2f size, float rotation, Vector2f rotationCenter) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    if (rotation != 0.f) {
        model = glm::translate(model, glm::vec3(rotationCenter.x * size.x, rotationCenter.y * size.y, 0.0f));
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, -glm::vec3(rotationCenter.x * size.x, rotationCenter.y * size.y, 0.0f));
    }
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

    return model;
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
    
    glm::mat4 projection = createProjectionMatrix();
    shaderToUse->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model;
    if (absoluteSize) {
        model = createModelMatrix(position, size, rotation, rotationCenter);
    }
    else {
        Vector2f trueSize = Vector2f(
            size.x * texture->getWidth() * sourceRectangle.width(), 
            size.y * texture->getHeight() * sourceRectangle.height()
        );
        Vector2f trueRotationCenter = Vector2f(
            rotationCenter.x * size.x * texture->getWidth() * sourceRectangle.width(), 
            rotationCenter.y * size.y * texture->getHeight() * sourceRectangle.height()
        );
        model = createModelMatrix(position, trueSize, rotation, trueRotationCenter);
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
    
    glm::mat4 projection = createProjectionMatrix();
    shaderToUse->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model;
    if (absoluteSize) {
        model = createModelMatrix(position, size, rotation, rotationCenter);
    }
    else {
        Vector2f trueSize = Vector2f(
            size.x * texture->getWidth() * sourceRectangle.width(), 
            size.y * texture->getHeight() * sourceRectangle.height()
        );
        Vector2f trueRotationCenter = Vector2f(
            rotationCenter.x * size.x * texture->getWidth() * sourceRectangle.width(), 
            rotationCenter.y * size.y * texture->getHeight() * sourceRectangle.height()
        );
        model = createModelMatrix(position, trueSize, rotation, trueRotationCenter);
    }

    shaderToUse->setUniformMatrix4("u_modelMatrix", model);
    shaderToUse->setUniformVector4f("u_color", Vector4f(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
    shaderToUse->setupForDraw();
    vertexArray->draw();    
}

void RenderTexture::draw(Canvas* canvas) {
    Window* windowInstance = Window::GetInstance();
    if (windowInstance == nullptr) {
        return;
    }
    glm::mat4 projection = createProjectionMatrix(false, false);
    Rectanglef displayRect = Rectanglef(
        canvas->getDisplaySpace().x() * windowInstance->getCoordinateScale().x,
        canvas->getDisplaySpace().y() * windowInstance->getCoordinateScale().y,
        canvas->getDisplaySpace().width() * windowInstance->getCoordinateScale().x,
        canvas->getDisplaySpace().height() * windowInstance->getCoordinateScale().y
    );
    
    glm::mat4 model = createModelMatrix(
        displayRect.position,
        displayRect.size,
        0, Vector2f(0, 0)
    );
    

    Shader* shaderToUse = canvas->m_DisplayShader;
    shaderToUse->setUniformMatrix4("u_projectionMatrix", projection);
    shaderToUse->setUniformMatrix4("u_modelMatrix", model);
    shaderToUse->setUniformVector4f(
        "u_color", 
        Vector4f(
            canvas->m_Color.r / 255.f, 
            canvas->m_Color.g / 255.f, 
            canvas->m_Color.b / 255.f, 
            canvas->m_Color.a / 255.f
        )
    );
    shaderToUse->setRenderTexture("u_texture0", 0, *canvas);
    shaderToUse->setUniformVector4f("u_textureSource", Vector4f(0, 0, 1, 1));
    shaderToUse->setupForDraw();
    RenderTexture::DefaultVertexArray->draw();    
}

#include <glad/glad.h>


#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

RenderTexture::RenderTexture(unsigned int width, unsigned int height) {
    m_Resolution = Vector2u(width, height);
    link();
}

RenderTexture::~RenderTexture() {
    unlink();
    delete m_RenderTexture;
    m_RenderTexture = nullptr;
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
}

void RenderTexture::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int RenderTexture::getWidth() {
    return m_Resolution.x;
}

int RenderTexture::getHeight() {
    return m_Resolution.y;
}

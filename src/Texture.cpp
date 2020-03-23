#include <glad/glad.h>

#include "Mantaray/GLObjects/Texture.hpp"
#include "Mantaray/Graphics/Image.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

Texture::Texture() {
    //generateTextureID();
    link();
}

Texture::Texture(std::string pathToTexture) {
    //generateTextureID();
    link();
    Image i = Image(pathToTexture);
    uploadTextureData(i.m_ImageData, i.getWidth(), i.getHeight(), i.m_NrChannels);
}

Texture::Texture(Image &image) {
    //generateTextureID();
    link();
    uploadTextureData(image.m_ImageData, image.getWidth(), image.getHeight(), image.m_NrChannels);
}

Texture::~Texture() {
    unlink();
}

void Texture::setFromImage(Image &image) {
    uploadTextureData(image.m_ImageData, image.getWidth(), image.getHeight(), image.m_NrChannels);
}

int Texture::getWidth() {
    return m_Size.x;
}

int Texture::getHeight() {
    return m_Size.y;
}

unsigned int Texture::getTextureID() {
    return m_TextureID;
}

void Texture::allocate() {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::release() {
    glDeleteTextures(1, &m_TextureID);
}

void Texture::uploadTextureData(unsigned char* textureData, int width, int height, int nrChannels) {
    m_Size = Vector2<int>(width, height);

    unsigned int format = 0;
    switch (nrChannels) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            Logger::Log("Texture", "Unsupported number of channels", Logger::LOG_WARNING);
            return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);   
}

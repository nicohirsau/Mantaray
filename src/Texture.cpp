#include <glad/glad.h>

#include "Mantaray/Graphics/Texture.h"
#include "Mantaray/Graphics/Image.h"
#include "Mantaray/Core/Logger.h"

using namespace MR;

Texture::Texture() {
    generateTextureID();
}

Texture::Texture(std::string pathToTexture) {
    generateTextureID();
    Image i = Image(pathToTexture);
    uploadTextureData(i.m_ImageData, i.getWidth(), i.getHeight(), i.m_NrChannels);
}

Texture::Texture(Image &image) {
    generateTextureID();
    uploadTextureData(image.m_ImageData, image.getWidth(), image.getHeight(), image.m_NrChannels);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_TextureID);
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

void Texture::generateTextureID() {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::uploadTextureData(unsigned char* textureData, int width, int height, int nrChannels) {
    m_Size = Vector2i(width, height);

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

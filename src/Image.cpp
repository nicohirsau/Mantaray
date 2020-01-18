#include <algorithm>
#include <cstdlib>

#include "Mantaray/Graphics/Image.h"
#include "Mantaray/Graphics/Texture.h"
#include "Mantaray/Core/FileSystem.h"

using namespace MR;

Image::Image() {}

Image::Image(std::string pathToImage) {
    loadFromFile(pathToImage);
}

Image::Image(unsigned char* imageData, int width, int height, int nrChannels) {
    m_ImageData = new unsigned char[width * height * nrChannels];
    for (int i = 0; i < width * height *nrChannels; i++) {
        m_ImageData[i] = imageData[i];
    }
    m_Size = Vector2i(width, height);
    m_NrChannels = nrChannels;
}

Image::~Image() {
    unloadData();
}

void Image::loadFromFile(std::string pathToImage) {
    FileSystem::ReadImage(pathToImage, m_ImageData, m_Size.x, m_Size.y, m_NrChannels);
}

void Image::unloadData() {
    free(m_ImageData);
    m_ImageData = nullptr;
    m_Size = Vector2i(0, 0);
    m_NrChannels = 0;
}

void Image::setPixel(Vector2u coordinate, unsigned char colorValue) {
    int arrayIndex = ((int)coordinate.x + getWidth() * (int)coordinate.y) * m_NrChannels;
    for (int i = 0; i < m_NrChannels; i++) {
        m_ImageData[arrayIndex + i] = colorValue;
    }
}

void Image::setPixel(Vector2u coordinate, Color color) {
    int arrayIndex = ((int)coordinate.x + getWidth() * (int)coordinate.y) * m_NrChannels;

    switch (m_NrChannels)
    {
    case 1:
        m_ImageData[arrayIndex] = color.r;
        break;
    case 2:
        m_ImageData[arrayIndex] = color.r;
        m_ImageData[arrayIndex + 1] = color.g;
        break;
    case 3:
        m_ImageData[arrayIndex] = color.r;
        m_ImageData[arrayIndex + 1] = color.g;
        m_ImageData[arrayIndex + 2] = color.b;
        break;
    case 4:
        m_ImageData[arrayIndex] = color.r;
        m_ImageData[arrayIndex + 1] = color.g;
        m_ImageData[arrayIndex + 2] = color.b;
        m_ImageData[arrayIndex + 3] = color.a;
        break;
    
    default:
        break;
    }
}

int Image::getWidth() {
    return m_Size.x;
}

int Image::getHeight() {
    return m_Size.y;
}

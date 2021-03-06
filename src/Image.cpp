#include <algorithm>
#include <cstdlib>

#include "Mantaray/Core/Image.hpp"
#include "Mantaray/OpenGL/Objects/Texture.hpp"
#include "Mantaray/Core/FileSystem.hpp"

using namespace MR;

Image::Image() {
    m_ImageData = new unsigned char[0];
}

Image::Image(std::string pathToImage, bool flipVertically) {
    loadFromFile(pathToImage, flipVertically);
}

Image::Image(std::vector<unsigned char>& imageData, unsigned int width, unsigned int height, int nrChannels) {
    if (m_ImageData != nullptr)
        unloadData();
    m_ImageData = new unsigned char[width * height * nrChannels];

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            int targetArrayIndex = (x + width * y) * nrChannels;
            int sourceArrayIndex = (x + width * (height - y - 1)) * nrChannels;
            for (int i = 0; i < nrChannels; i++) {
                m_ImageData[targetArrayIndex + i] = imageData[sourceArrayIndex + i];
            }
        }
    }
    
    m_Size = Vector2u(width, height);
    m_NrChannels = nrChannels;
}

Image::~Image() {
    unloadData();
}

void Image::loadFromFile(std::string pathToImage, bool flipVertically) {
    if (m_ImageData != nullptr)
        unloadData();
    int width, height;
    FileSystem::ReadImage(pathToImage, m_ImageData, width, height, m_NrChannels, flipVertically);
    m_Size = Vector2u(width, height);
}

void Image::unloadData() {
    free(m_ImageData);
    m_ImageData = nullptr;
    m_Size = Vector2u(0, 0);
    m_NrChannels = 0;
}

Color Image::getPixel(Vector2u coordinate) {
    Color pixelColor;
    int arrayIndex = ((int)coordinate.x + getWidth() * (getHeight() - (int)coordinate.y - 1)) * m_NrChannels;
    switch (m_NrChannels) {
        case 1:
            pixelColor.r = m_ImageData[arrayIndex];
            pixelColor.g = m_ImageData[arrayIndex];
            pixelColor.b = m_ImageData[arrayIndex];
            pixelColor.a = 0xFF;
            break;
        case 2:
            pixelColor.r = m_ImageData[arrayIndex];
            pixelColor.g = m_ImageData[arrayIndex + 1];
            pixelColor.b = 0x00;
            pixelColor.a = 0xFF;
            break;
        case 3:
            pixelColor.r = m_ImageData[arrayIndex];
            pixelColor.g = m_ImageData[arrayIndex + 1];
            pixelColor.b = m_ImageData[arrayIndex + 2];
            pixelColor.a = 0xFF;
            break;
        case 4:
            pixelColor.r = m_ImageData[arrayIndex];
            pixelColor.g = m_ImageData[arrayIndex + 1];
            pixelColor.b = m_ImageData[arrayIndex + 2];
            pixelColor.a = m_ImageData[arrayIndex + 3];
            break;
        default:
            break;
    }
    return pixelColor;
}

void Image::setPixel(Vector2u coordinate, unsigned char colorValue) {
    int arrayIndex = ((int)coordinate.x + getWidth() * (getHeight() - (int)coordinate.y - 1)) * m_NrChannels;
    for (int i = 0; i < m_NrChannels; i++) {
        m_ImageData[arrayIndex + i] = colorValue;
    }
}

void Image::setPixel(Vector2u coordinate, Color color) {
    int arrayIndex = ((int)coordinate.x + getWidth() * (getHeight() - (int)coordinate.y - 1)) * m_NrChannels;

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

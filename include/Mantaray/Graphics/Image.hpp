#pragma once

#include <string>
#include <vector>

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"

namespace MR {
class Texture;

class Image {
    friend class Texture;

    public:
        Image();
        Image(std::string pathToImage);
        Image(std::vector<unsigned char>& imageData, unsigned int width, unsigned int height, int nrChannels);
        ~Image();

        void loadFromFile(std::string pathToImage);
        void unloadData();

        void setPixel(Vector2u coordinate, unsigned char colorValue);
        void setPixel(Vector2u coordinate, Color color);

        int getWidth();
        int getHeight();

    private:
        unsigned char* m_ImageData = nullptr;
        Vector2u m_Size = Vector2u(0, 0);
        int m_NrChannels = 0;
};
}
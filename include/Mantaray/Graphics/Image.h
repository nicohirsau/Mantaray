#include <string>
#include <vector>

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.h"

namespace MR {
class Texture;

class Image {
    friend class Texture;

    public:
        Image();
        Image(std::string pathToImage);
        Image(std::vector<unsigned char>& imageData, int width, int height, int nrChannels);
        ~Image();

        void loadFromFile(std::string pathToImage);
        void unloadData();

        void setPixel(Vector2<unsigned int> coordinate, unsigned char colorValue);
        void setPixel(Vector2<unsigned int> coordinate, Color color);

        int getWidth();
        int getHeight();

    private:
        unsigned char* m_ImageData = nullptr;
        Vector2<int> m_Size = Vector2<int>(0, 0);
        int m_NrChannels = 0;
};
}
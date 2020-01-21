#include <string>

#include "Mantaray/Core/Vector.h"
#include "Mantaray/Core/Color.h"

namespace MR {
class Texture;

class Image {
    friend class Texture;

    public:
        Image();
        Image(std::string pathToImage);
        Image(unsigned char* imageData, int width, int height, int nrChannels);
        ~Image();

        void loadFromFile(std::string pathToImage);
        void unloadData();

        void setPixel(Vector2u coordinate, unsigned char colorValue);
        void setPixel(Vector2u coordinate, Color color);

        int getWidth();
        int getHeight();

    private:
        unsigned char* m_ImageData = nullptr;
        Vector2i m_Size = Vector2i(0, 0);
        int m_NrChannels = 0;
};
}
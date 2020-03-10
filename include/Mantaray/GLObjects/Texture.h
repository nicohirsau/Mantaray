#include <string>

#include "Mantaray/Core/Vector.h"

namespace MR {
class Image;

class Texture {
    public:
        Texture();
        Texture(std::string pathToTexture);
        Texture(Image &image);
        ~Texture();

        void setFromImage(Image &image);

        int getWidth();
        int getHeight();
        unsigned int getTextureID();

    private:
        void generateTextureID();
        void uploadTextureData(unsigned char* textureData, int width, int height, int nrChannels);

    private:
        unsigned int m_TextureID = 0;
        Vector2i m_Size = Vector2i(0, 0);
};
}
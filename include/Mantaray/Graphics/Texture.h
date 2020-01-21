#include <string>

#include "Mantaray/Core/Vector.h"

namespace MR {
class Image;

class Texture {
    public:
        Texture();
        Texture(std::string pathToTexture);
        Texture(unsigned char* textureData, int width, int height, int nrChannels);
        ~Texture();

        void setFromImage(Image &image);

        int getWidth();
        int getHeight();
        unsigned int getTextureID();

    private:
        void uploadTextureData(unsigned char* textureData, int width, int height, int nrChannels);

    private:
        bool m_HasTextureID = false;
        unsigned int m_TextureID;
        Vector2i m_Size;
};
}
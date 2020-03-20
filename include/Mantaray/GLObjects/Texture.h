#pragma once

#include <string>

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/GLObjects/GLObject.h"

namespace MR {
class Image;

class Texture : public GLObject {
    public:
        Texture();
        Texture(std::string pathToTexture);
        Texture(Image &image);
        ~Texture();

        void setFromImage(Image &image);

        int getWidth();
        int getHeight();
        unsigned int getTextureID();

    protected:
        void allocate() override;
        void release() override;

    private:
        void uploadTextureData(unsigned char* textureData, int width, int height, int nrChannels);

    private:
        unsigned int m_TextureID = 0;
        Vector2<int> m_Size = Vector2<int>(0, 0);
};
}
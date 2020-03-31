#pragma once

#include <string>

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/GLObjects/GLObject.hpp"

namespace MR {
class Image;

class Texture : public GLObject {
    friend class RenderTexture;

    public:
        Texture();
        Texture(std::string pathToTexture);
        Texture(Image &image);
        Texture(Vector2u resolution, int channels = 4);
        ~Texture();

        void setFromImage(Image &image);

        int getWidth();
        int getHeight();
        unsigned int getTextureID();

        void bind() override;
        void unbind() override;

    protected:
        void allocate() override;
        void release() override;

    private:
        void uploadTextureData(unsigned char* textureData, int width, int height, int nrChannels);

    private:
        unsigned int m_TextureID = 0;
        Vector2i m_Size = Vector2i(0, 0);
};
}

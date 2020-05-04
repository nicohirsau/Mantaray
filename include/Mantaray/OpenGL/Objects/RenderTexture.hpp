#pragma once

#include "Mantaray/OpenGL/Object.hpp"
#include "Mantaray/OpenGL/Objects/Texture.hpp"
#include "Mantaray/Core/Vector.hpp"

namespace MR {
class RenderTexture : public Object{
    friend class Shader;
    friend class Canvas;
    
    public:
        RenderTexture(unsigned int width, unsigned int height);
        ~RenderTexture();

        void bind() override;
        void unbind() override;

        int getWidth();
        int getHeight();
    
    protected:
        void allocate() override;
        void release() override;
    
    private:
        unsigned int m_FBO, m_RBO;
        Vector2u m_Resolution;
        Texture *m_RenderTexture;
};
}

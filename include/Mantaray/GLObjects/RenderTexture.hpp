#include "Mantaray/GLObjects/GLObject.hpp"
#include "Mantaray/GLObjects/Texture.hpp"
#include "Mantaray/Core/Vector.hpp"

namespace MR {
class RenderTexture : public GLObject{
    friend class Shader;
    
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
        Vector2<unsigned int> m_Resolution;
        Texture *m_RenderTexture;
};
}
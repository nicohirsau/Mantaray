#pragma once

#include "Mantaray/OpenGL/Object.hpp"
#include "Mantaray/OpenGL/Objects/Texture.hpp"
#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/Core/Shapes.hpp"

namespace MR {
class RenderTexture : public Object {
    friend class Shader;
    friend class Canvas;
    
    public:
        RenderTexture(Vector2u resolution);
        RenderTexture(Vector2u resolution, Vector2f coordinateScale);
        ~RenderTexture();

        void bind() override;
        void unbind() override;

        int getWidth();
        int getHeight();

        Vector2f getCoordinateScale();
        void setCoordinateScale(Vector2f coordinateScale);
        
        Vector2f getOffset();
        void setOffset(Vector2f offset);
        void addOffset(Vector2f offset);

        float getScale();
        void setScale(float scale);

        Vector2f getScaleCenter();
        void setScaleCenter(Vector2f scaleCenter);

        void clear(Color color = Color(0x00u));

        void draw(struct Sprite& sprite);
        void draw(
            class Texture* texture, 
            Vector2f position = Vector2f(0, 0), 
            Vector2f size = Vector2f(1, 1),
            bool absoluteSize = true,
            float rotation = 0,
            Vector2f rotationCenter = Vector2f(0, 0),
            Rectanglef sourceRectangle = Rectanglef(0, 0, 1, 1),
            Color color = Color(0xFFu),
            class Shader* shader = nullptr
        );
        void draw(struct Polygon& polygon);
        void draw(
            class VertexArray* vertexArray,
            Vector2f position = Vector2f(0, 0), 
            Vector2f size = Vector2f(1, 1),
            bool absoluteSize = true,
            float rotation = 0,
            Vector2f rotationCenter = Vector2f(0, 0),
            Color color = Color(0xFFu),
            class Shader* shader = nullptr,
            class Texture* texture = nullptr, 
            Rectanglef sourceRectangle = Rectanglef(0, 0, 1, 1)
        );
    
    protected:
        void allocate() override;
        void release() override;
    
    private:
        void initializeDefaults();
    
    protected:
        unsigned int m_FBO, m_RBO;
        Vector2u m_Resolution;
        Texture *m_RenderTexture;

        Vector2f m_CoordinateScale = Vector2f(1, 1);
        Vector2f m_Offset = Vector2f(0, 0);
        float m_Scale = 1.0f;
        Vector2f m_ScaleCenter = Vector2f(.5f, .5f);
        
        static class VertexArray* DefaultVertexArray;
        static class Shader* DefaultTexturedShader;
        static class Shader* DefaultColoredShader;
};
}

#pragma once

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/Core/Shape.hpp"

namespace MR {
class GLCanvas {
    public:
        GLCanvas(Vector2u resolution, Vector2f coordinateScale);
        GLCanvas(Vector2u resolution);
        ~GLCanvas();

        class Shader* getShader();
        void setShader(class Shader* shader);
        class RenderTexture* getRenderTexture();

        void setCoordinateScale(Vector2f coordinateScale);
        Vector2f getCoordinateScale();
        
        void setOffset(Vector2f offset);
        void addOffset(Vector2f offset);
        Vector2f getOffset();

        void bind();
        void bind(Color color);
        void unbind();
        void clear(Color color = Color(0x00u));
        void display(Rectanglei viewPort);
        void display(Rectanglei viewPort, Rectanglef destination);

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

    private:
        void init(Vector2u resolution, Vector2f coordinateScale);

    private:
        class RenderTexture* m_RenderTexture = nullptr;
        class Shader* m_Shader = nullptr;
        Vector2f m_CoordinateScale = Vector2f(1, 1);
        Vector2f m_Offset = Vector2f(0, 0);
        
        static class VertexArray* DefaultVertexArray;
        static class Shader* DefaultTexturedShader;
        static class Shader* DefaultColoredShader;
};
}
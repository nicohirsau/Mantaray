#pragma once

#include "Mantaray/GLObjects/VertexArray.hpp"
#include "Mantaray/GLObjects/Texture.hpp"
#include "Mantaray/GLObjects/Shader.hpp"
#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/Core/Shapes.hpp"

namespace MR {
struct Drawable {
    public:
        Texture* texture = nullptr;
        Vector2f position = Vector2f(0, 0);
        Vector2f size = Vector2f(1, 1);
        Color color = Color(0xFF);
        bool absoluteSize = true;
        float rotation = 0;
        Vector2f rotationCenter = Vector2f(0, 0);
        Rectanglef sourceRectangle = Rectanglef(0, 0, 1, 1);
        Shader* shader = nullptr;
};

struct Sprite : public Drawable {
    Sprite(Texture* texture) {
        this->texture = texture;
        this->absoluteSize = false;
    }
};

struct Polygon : public Drawable {
    public:
        Polygon() {
            this->vertexArray = new VertexArray();
            this->shouldDeleteVA = true;
        }
        Polygon(VertexArray* vertexArray, bool shouldDeleteVA = false) {
            this->vertexArray = vertexArray;
            this->shouldDeleteVA = shouldDeleteVA;
        }
        ~Polygon() {
            if (this->shouldDeleteVA) {
                delete this->vertexArray;
            }
        }

    public:
        VertexArray* vertexArray = nullptr;
    
    private:
        bool shouldDeleteVA = false;
};
}

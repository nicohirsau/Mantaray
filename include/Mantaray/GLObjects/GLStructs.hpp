#include "Mantaray/GLObjects/VertexArray.hpp"
#include "Mantaray/GLObjects/Texture.hpp"
#include "Mantaray/GLObjects/Shader.hpp"
#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/Core/Shape.hpp"

namespace MR {
struct Sprite {
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

struct Mesh {

    public:
        VertexArray* vertexArray = nullptr;
        Vector2f position = Vector2f(0, 0);
        Vector2f size = Vector2f(1, 1);
        Color color = Color(0xFF);
        bool absoluteSize = true;
        float rotation = 0;
        Vector2f rotationCenter = Vector2f(0, 0);
        Texture* texture = nullptr;
        Rectanglef sourceRectangle = Rectanglef(0, 0, 1, 1);
        Shader* shader = nullptr;
};
}
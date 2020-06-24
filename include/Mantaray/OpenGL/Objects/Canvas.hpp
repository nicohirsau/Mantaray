#pragma once

#include "Mantaray/Core/Shapes.hpp"
#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"

namespace MR {
class Canvas : public RenderTexture {
    friend class RenderTexture;

    public:
        Canvas(Vector2u resolution);
        Canvas(Vector2u resolution, Rectanglef displaySpace);
        Canvas(Vector2u resolution, Vector2f coordinateScale);
        Canvas(Vector2u resolution, Vector2f coordinateScale, Rectanglef displaySpace);

        Rectanglef getDisplaySpace();
        void setDisplaySpace(Rectanglef displaySpace);

        Vector2d getMousePosition();

    private:
        Rectanglef m_DisplaySpace = Rectanglef(0, 0, 1, 1);
        class Shader* m_DisplayShader = nullptr;
        Color m_Color = Color(0xFF);
};
}
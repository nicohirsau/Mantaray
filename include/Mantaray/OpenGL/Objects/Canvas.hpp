#pragma once

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/Core/Color.hpp"
#include "Mantaray/Core/Shapes.hpp"
#include "Mantaray/OpenGL/Objects/RenderTexture.hpp"

namespace MR {
class Canvas : public RenderTexture {
    public:
        Canvas(Vector2u resolution, Vector2f coordinateScale);
        Canvas(Vector2u resolution);
        ~Canvas();
        
        class Shader* getShader();
        void setShader(class Shader* shader);

        Rectanglef getViewPortDestination();
        void setViewPortDestination(Rectanglef viewPortDestination);

        void display();
    
    private:
        class Shader* m_Shader = nullptr;
        Rectanglef m_ViewPortDestination = Rectanglef(0, 0, 1, 1);
};
}

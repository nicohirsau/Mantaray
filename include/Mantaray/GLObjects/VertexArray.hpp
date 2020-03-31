#pragma once

#include <vector>

#include "Mantaray/Core/Vector.hpp"
#include "Mantaray/GLObjects/GLObject.hpp"

namespace MR {

class VertexArray : public GLObject{
    public:
        VertexArray();
        ~VertexArray();
        void addVertice(Vector2f v);
        void addVertices(Vector2f v[], unsigned int vc);
        void addVertices(std::vector<Vector2f> v);
        void addIndex(int i);
        void addIndices(std::vector<int> i);
        void addTextureCoordinate(Vector2f c);
        void addTextureCoordinates(std::vector<Vector2f> c);
        void clear();
        void uploadVertexArrayData();
        void draw();

        void bind() override;
        void unbind() override;

    protected:
        void allocate() override;
        void release() override;

    private:
        unsigned int m_VAO, m_VBO, m_TCBO, m_EBO;
        std::vector<Vector2f> m_Vertices;

        bool m_UsesIndices = false;
        std::vector<int> m_Indices;

        bool m_UsesTextureCoordinates = false;
        std::vector<Vector2f> m_TextureCoordinates;
};
}

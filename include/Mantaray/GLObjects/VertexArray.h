#pragma once

#include <vector>

#include "Mantaray/Core/Vector.h"
#include "Mantaray/GLObjects/GLObject.h"

namespace MR {

class VertexArray : public GLObject{
    public:
        VertexArray();
        ~VertexArray();
        void addVertice(Vector2f v);
        void addVertices(Vector2f v[], unsigned int vc);
        void addVertices(std::vector<Vector2f> v);
        void addIndex(int i);
        void clear();
        void uploadVertexArrayData();
        void draw();

    protected:
        void allocate() override;
        void release() override;

    private:
        unsigned int m_VAO, m_VBO, m_EBO;
        std::vector<Vector2f> m_Vertices;

        bool m_UsesIndices = false;
        std::vector<int> m_Indices;
};
}

#include <glad/glad.h>

#include "Mantaray/OpenGL/Context.hpp"
#include "Mantaray/OpenGL/Objects/VertexArray.hpp"

using namespace MR;

VertexArray::VertexArray() {
    link();
    m_Vertices = std::vector<Vector2f>();
    m_Indices = std::vector<int>();
}

VertexArray::~VertexArray() {
    unlink();
}

void VertexArray::allocate() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

void VertexArray::release() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    if (m_UsesIndices) {
        glDeleteBuffers(1, &m_EBO);
    }
    if (m_UsesTextureCoordinates) {
        glDeleteBuffers(1, &m_TCBO);
    }
}

void VertexArray::bind() {
    Context::BindVertexArray(m_VAO);
}

void VertexArray::unbind() {
    Context::BindVertexArray(0);
}

void VertexArray::uploadVertexArrayData() {
    bind();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size() * 2, &m_Vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (m_UsesIndices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);
    }

    if(m_UsesTextureCoordinates) {
        glBindBuffer(GL_ARRAY_BUFFER, m_TCBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_TextureCoordinates.size() * 2, &m_TextureCoordinates[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }
}

void VertexArray::draw() {
    bind();

    if (m_UsesIndices) {
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, (void*)0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    }
}

void VertexArray::addVertice(Vector2f v) {
    m_Vertices.push_back(v);
}

void VertexArray::addVertices(Vector2f v[], unsigned int vc) {
    for (unsigned int i = 0; i < vc; i++) {
        addVertice(v[i]);
    }
}

void VertexArray::addVertices(std::vector<Vector2f> v) {
    m_Vertices.insert(m_Vertices.end(), v.begin(), v.end());
}

void VertexArray::addIndex(int i) {
    if (!m_UsesIndices) {
        glGenBuffers(1, &m_EBO);
        m_UsesIndices = true;
    }
    m_Indices.push_back(i);
}

void VertexArray::addIndices(std::vector<int> i) {
    for (int index : i) {
        addIndex(index);
    }
}

void VertexArray::addTextureCoordinate(Vector2f c) {
    if (!m_UsesTextureCoordinates) {
        glGenBuffers(1, &m_TCBO);
        m_UsesTextureCoordinates = true;
    }
    m_TextureCoordinates.push_back(c);
}

void VertexArray::addTextureCoordinates(std::vector<Vector2f> c) {
    for (Vector2f coordinate : c) {
        addTextureCoordinate(coordinate);
    }
}

void VertexArray::clear() {
    m_Vertices.clear();
    m_Indices.clear();
    m_TextureCoordinates.clear();
}

#include "Mantaray/Core/Mesh.h"
#include <glad/glad.h>

using namespace MR;

Mesh::Mesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    m_Vertices = std::vector<Vector2f>();
    m_Indices = std::vector<int>();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Mesh::uploadMeshData() {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Vertices.size() * 2, &m_Vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Mesh::draw() {
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(m_Vertices) / sizeof(float));
}

void Mesh::addVertice(Vector2f v) {
    m_Vertices.push_back(v);
}

void Mesh::addVertices(Vector2f v[], unsigned int vc) {
    for (unsigned int i = 0; i < vc; i++) {
        addVertice(v[i]);
    }
}

void Mesh::addVertices(std::vector<Vector2f> v) {
    m_Vertices.insert(m_Vertices.end(), v.begin(), v.end());
}

void Mesh::addIndex(int i) {
    if (!m_UsesIncices)
        m_UsesIncices = true;
    m_Indices.push_back(i);
}

void Mesh::clear() {
    m_Vertices.clear();
    m_Indices.clear();
}

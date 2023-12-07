#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {
    shader = new Shader("shader.vert", "shader.frag");

    glEnable(GL_DEPTH_TEST);

    shader->use();

    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &elementBufferObject);
}

MeshRenderer::MeshRenderer(Shader *shader) {
    this->shader = shader;
    usesExternalShader = true;

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &elementBufferObject);
}

int MeshRenderer::addVertexToMesh(const Vector3& position, const Vector3& normal, const Vector3& color) {
    vertices.push_back(position.x);
    vertices.push_back(position.y);
    vertices.push_back(position.z);

    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);

    vertices.push_back(color.x);
    vertices.push_back(color.y);
    vertices.push_back(color.z);

    return (vertices.size() / 9) - 1;
}

void MeshRenderer::addTriangleToMesh(unsigned int v1, unsigned int v2, unsigned int v3) {
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);
}

void MeshRenderer::setProjectionMatrix(const Matrix4& matrix) {
    projection = matrix;
}


void MeshRenderer::setViewMatrix(const Matrix4& matrix) {
    view = matrix;
}

void MeshRenderer::setModelMatrix(const Matrix4& matrix) {
    model = matrix;
}

bool MeshRenderer::isEmpty() {
    return vertices.empty();
}

void MeshRenderer::clear() {
    vertices.clear();
    indices.clear();
}

void MeshRenderer::render() {
    shader->setMatrix4("model", model);
    shader->setMatrix4("view", view);
    shader->setMatrix4("projection", projection);

    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(shader->getAttribLocation("aPosition"));
    glVertexAttribPointer(shader->getAttribLocation("aPosition"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);

    glEnableVertexAttribArray(shader->getAttribLocation("aNormal"));
    glVertexAttribPointer(shader->getAttribLocation("aNormal"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<GLvoid*>(3 * sizeof(float)));

    glEnableVertexAttribArray(shader->getAttribLocation("aColor"));
    glVertexAttribPointer(shader->getAttribLocation("aColor"), 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<GLvoid*>(6 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

MeshRenderer::~MeshRenderer() {
    if (!usesExternalShader) {
        delete shader;
    }
}

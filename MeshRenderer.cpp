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

int MeshRenderer::addVertexToMesh(Vector3 position, Vector3 normal) {
    vertices.push_back(position.x);
    vertices.push_back(position.y);
    vertices.push_back(position.z);

    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);

    return (vertices.size() / 6) - 1;
}

void MeshRenderer::addTriangleToMesh(unsigned int v1, unsigned int v2, unsigned int v3) {
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);
}

void MeshRenderer::setProjectionMatrix(Matrix4 matrix) {
    projection = matrix;
}


void MeshRenderer::setViewMatrix(Matrix4 matrix) {
    view = matrix;
}

void MeshRenderer::setModelMatrix(Matrix4 matrix) {
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL - normal; GL_LINE - wireframe

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(shader->getAttribLocation("aPosition"));
    glVertexAttribPointer(shader->getAttribLocation("aPosition"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);

    glEnableVertexAttribArray(shader->getAttribLocation("aNormal"));
    glVertexAttribPointer(shader->getAttribLocation("aNormal"), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<GLvoid*>(3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

MeshRenderer::~MeshRenderer() {
    if (!usesExternalShader) {
        delete shader;
    }
}

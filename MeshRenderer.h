#ifndef PAPER1_MESHRENDERER_H
#define PAPER1_MESHRENDERER_H

#include <vector>
#include "Matrix4.h"
#include "Shader.h"

class MeshRenderer {
private:
    std::vector<float> vertices = std::vector<float>();
    std::vector<unsigned int> indices = std::vector<unsigned int>();

    GLuint vertexArrayObject;
    GLuint vertexBufferObject;
    GLuint elementBufferObject;

    Matrix4 model = Matrix4::createTranslation(Vector3(0.0f, 0.0f, 0.0f));
    Matrix4 view = Matrix4::createTranslation(Vector3(0.0f, 0.0f, 0.0f));
    Matrix4 projection = Matrix4::createPerspectiveFieldOfView(45.0f * M_PI / 180.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    Shader *shader;
    bool usesExternalShader;

public:
    MeshRenderer();
    MeshRenderer(Shader *shader);

    int addVertexToMesh(Vector3 position, Vector3 normal);
    void addTriangleToMesh(unsigned int v1, unsigned int v2, unsigned int v3);

    void setProjectionMatrix(Matrix4 matrix);
    void setViewMatrix(Matrix4 matrix);
    void setModelMatrix(Matrix4 matrix);

    bool isEmpty();

    void clear();

    void render();

    ~MeshRenderer();
};


#endif //PAPER1_MESHRENDERER_H

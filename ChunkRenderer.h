#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include <vector>

#include "Chunk.h"
#include "MeshRenderer.h"

class ChunkRenderer {
private:
    std::vector<MeshRenderer> _meshRenderers = std::vector<MeshRenderer>();
    std::vector<int> _meshIds = std::vector<int>();

    Matrix4 _view;
    Matrix4 _projection;

    Shader _shader = Shader("shader.vert", "shader.frag");

    void createCube(bool xNegative, bool xPositive, bool yNegative, bool yPositive, bool zNegative, bool zPositive, float x, float y, float z, MeshRenderer renderer);

public:
    ChunkRenderer();

    void setViewMatrix(Matrix4 matrix);
    void setProjectionMatrix(Matrix4 matrix);
    bool isEmpty(int meshId);
    int addChunk(Chunk chunk);
    void removeChunk(int id);
    void render(int meshId);

    ~ChunkRenderer();
};



#endif //CHUNKRENDERER_H

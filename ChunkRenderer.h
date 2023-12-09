#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include <vector>

#include "Block.h"
#include "MeshRenderer.h"
#include "Vector3i.h"

class ChunkRenderer {
private:
    static constexpr int CHUNK_SIZE = 16;

    std::vector<MeshRenderer*> _meshRenderers = std::vector<MeshRenderer*>();
    std::vector<int> _meshIds = std::vector<int>();

    Matrix4 _view;
    Matrix4 _projection;

    Shader *_shader;

    static void createCube(bool xNegative, bool xPositive, bool yNegative, bool yPositive, bool zNegative, bool zPositive, float x, float y, float z, Vector3 color, MeshRenderer* renderer);

public:
    ChunkRenderer();

    void setViewMatrix(const Matrix4& matrix);
    void setProjectionMatrix(const Matrix4& matrix);
    bool isEmpty(int meshId);
    int addChunk(const Vector3i& position, Block*[][CHUNK_SIZE][CHUNK_SIZE]);
    int addChunk(MeshRenderer* renderer);
    MeshRenderer* createChunkMeshToAdd(const Vector3i& position, Block*[][CHUNK_SIZE][CHUNK_SIZE]) const;
    void removeChunk(int id);
    void render(int meshId);

    ~ChunkRenderer();
};



#endif //CHUNKRENDERER_H

#include "ChunkRenderer.h"

#include <algorithm>

void ChunkRenderer::createCube(bool xNegative, bool xPositive, bool yNegative, bool yPositive, bool zNegative,
                               bool zPositive, float x, float y, float z, MeshRenderer renderer) {
    Vector3 p1 = Vector3(x, y, z + Block::BLOCK_RENDER_SIZE);
    Vector3 p2 = Vector3(x + Block::BLOCK_RENDER_SIZE, y, z + Block::BLOCK_RENDER_SIZE);
    Vector3 p3 = Vector3(x + Block::BLOCK_RENDER_SIZE, y + Block::BLOCK_RENDER_SIZE, z + Block::BLOCK_RENDER_SIZE);
    Vector3 p4 = Vector3(x, y + Block::BLOCK_RENDER_SIZE, z + Block::BLOCK_RENDER_SIZE);
    Vector3 p5 = Vector3(x + Block::BLOCK_RENDER_SIZE, y, z);
    Vector3 p6 = Vector3(x, y, z);
    Vector3 p7 = Vector3(x, y + Block::BLOCK_RENDER_SIZE, z);
    Vector3 p8 = Vector3(x + Block::BLOCK_RENDER_SIZE, y + Block::BLOCK_RENDER_SIZE, z);

    Vector3 n;

    unsigned int v1;
    unsigned int v2;
    unsigned int v3;
    unsigned int v4;
    unsigned int v5;
    unsigned int v6;
    unsigned int v7;
    unsigned int v8;

    // Front
    n = Vector3::unitZ();
    if (zPositive) {
        v1 = static_cast<unsigned int>(renderer.addVertexToMesh(p1, n));
        v2 = static_cast<unsigned int>(renderer.addVertexToMesh(p2, n));
        v3 = static_cast<unsigned int>(renderer.addVertexToMesh(p3, n));
        v4 = static_cast<unsigned int>(renderer.addVertexToMesh(p4, n));
        renderer.addTriangleToMesh(v1, v2, v3);
        renderer.addTriangleToMesh(v1, v3, v4);
    }

    // Back
    n = -Vector3::unitZ();
    if (zNegative) {
        v5 = static_cast<unsigned int>(renderer.addVertexToMesh(p5, n));
        v6 = static_cast<unsigned int>(renderer.addVertexToMesh(p6, n));
        v7 = static_cast<unsigned int>(renderer.addVertexToMesh(p7, n));
        v8 = static_cast<unsigned int>(renderer.addVertexToMesh(p8, n));
        renderer.addTriangleToMesh(v5, v6, v7);
        renderer.addTriangleToMesh(v5, v7, v8);
    }

    // Right
    n = Vector3::unitX();
    if (xPositive) {
        v2 = static_cast<unsigned int>(renderer.addVertexToMesh(p2, n));
        v5 = static_cast<unsigned int>(renderer.addVertexToMesh(p5, n));
        v8 = static_cast<unsigned int>(renderer.addVertexToMesh(p8, n));
        v3 = static_cast<unsigned int>(renderer.addVertexToMesh(p3, n));
        renderer.addTriangleToMesh(v2, v5, v8);
        renderer.addTriangleToMesh(v2, v8, v3);
    }

    // Left
    n = -Vector3::unitX();
    if (xNegative) {
        v6 = static_cast<unsigned int>(renderer.addVertexToMesh(p6, n));
        v1 = static_cast<unsigned int>(renderer.addVertexToMesh(p1, n));
        v4 = static_cast<unsigned int>(renderer.addVertexToMesh(p4, n));
        v7 = static_cast<unsigned int>(renderer.addVertexToMesh(p7, n));
        renderer.addTriangleToMesh(v6, v1, v4);
        renderer.addTriangleToMesh(v6, v4, v7);
    }

    // Top
    n = Vector3::unitY();
    if (yPositive) {
        v4 = static_cast<unsigned int>(renderer.addVertexToMesh(p4, n));
        v3 = static_cast<unsigned int>(renderer.addVertexToMesh(p3, n));
        v8 = static_cast<unsigned int>(renderer.addVertexToMesh(p8, n));
        v7 = static_cast<unsigned int>(renderer.addVertexToMesh(p7, n));
        renderer.addTriangleToMesh(v4, v3, v8);
        renderer.addTriangleToMesh(v4, v8, v7);
    }

    // Bottom
    n = -Vector3::unitY();
    if (yNegative) {
        v6 = static_cast<unsigned int>(renderer.addVertexToMesh(p6, n));
        v5 = static_cast<unsigned int>(renderer.addVertexToMesh(p5, n));
        v2 = static_cast<unsigned int>(renderer.addVertexToMesh(p2, n));
        v1 = static_cast<unsigned int>(renderer.addVertexToMesh(p1, n));
        renderer.addTriangleToMesh(v6, v5, v2);
        renderer.addTriangleToMesh(v6, v2, v1);
    }
}

ChunkRenderer::ChunkRenderer() {
    _shader.use();
}

void ChunkRenderer::setViewMatrix(Matrix4 matrix) {
    _view = matrix;
    for (auto renderer: _meshRenderers) {
        renderer.setViewMatrix(_view);
    }
}

void ChunkRenderer::setProjectionMatrix(Matrix4 matrix) {
    _projection = matrix;
    for (auto renderer: _meshRenderers) {
        renderer.setProjectionMatrix(_projection);
    }
}

bool ChunkRenderer::isEmpty(int meshId) {
    auto it = std::find(_meshIds.begin(), _meshIds.end(), meshId);
    if (it != _meshIds.end()) {
        int index = it - _meshIds.begin();
        return _meshRenderers[index].isEmpty();
    }
    return true;
}

int ChunkRenderer::addChunk(Chunk chunk) {
    MeshRenderer renderer = MeshRenderer(&_shader);
    renderer.setViewMatrix(_view);
    renderer.setProjectionMatrix(_projection);
    renderer.setModelMatrix(
        Matrix4::createTranslation(
            static_cast<Vector3>(chunk.position) * Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE));

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                if (!chunk.blocks[x][y][z]->active)
                    continue;

                bool xNegative = true;
                if (x > 0) xNegative = !chunk.blocks[x - 1][y][z]->active;

                bool xPositive = true;
                if (x < Chunk::CHUNK_SIZE - 1) xPositive = !chunk.blocks[x + 1][y][z]->active;

                bool yNegative = true;
                if (y > 0) yNegative = !chunk.blocks[x][y - 1][z]->active;

                bool yPositive = true;
                if (y < Chunk::CHUNK_SIZE - 1) yPositive = !chunk.blocks[x][y + 1][z]->active;

                bool zNegative = true;
                if (z > 0) zNegative = !chunk.blocks[x][y][z - 1]->active;

                bool zPositive = true;
                if (z < Chunk::CHUNK_SIZE - 1) zPositive = !chunk.blocks[x][y][z + 1]->active;

                createCube(xNegative, xPositive, yNegative, yPositive, zNegative, zPositive, x, y, z, renderer);
            }
        }
    }

    _meshRenderers.push_back(renderer);

    if (_meshIds.empty()) {
        return 1;
    }

    int max = *std::max_element(_meshIds.begin(), _meshIds.end()) + 1;

    _meshIds.push_back(max);

    return max;
}

void ChunkRenderer::removeChunk(int id) {
    auto it = std::find(_meshIds.begin(), _meshIds.end(), id);
    if (it != _meshIds.end()) {
        int index = it - _meshIds.begin();
        _meshIds.erase(it);
        _meshRenderers.erase(_meshRenderers.begin() + index);
    }
}

void ChunkRenderer::render(int meshId) {
    auto it = std::find(_meshIds.begin(), _meshIds.end(), meshId);
    if (it != _meshIds.end()) {
        int index = it - _meshIds.begin();
        _meshRenderers[index].render();
    }
}

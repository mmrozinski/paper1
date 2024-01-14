#include "ChunkRenderer.h"

#include <algorithm>

void ChunkRenderer::createCube(bool xNegative, bool xPositive, bool yNegative, bool yPositive, bool zNegative,
                               bool zPositive, float x, float y, float z, Vector3 color, Vector3i texturePos, MeshRenderer* renderer) {
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

    Vector3 textureIndexScale(0.5f * texturePos.x, 0.5f * texturePos.y, 0.0f);

    Vector3 topLeft = Vector3(0.0f, 0.5f, 0.0f) + textureIndexScale;
    Vector3 topRight = Vector3(0.5f, 0.5f, 0.0f) + textureIndexScale;
    Vector3 bottomLeft = Vector3(0.0f, 0.0f, 0.0f) + textureIndexScale;
    Vector3 bottomRight = Vector3(0.5f, 0.0f, 0.0f) + textureIndexScale;

    // Front
    n = Vector3::unitZ();
    if (zPositive) {
        v1 = static_cast<unsigned int>(renderer->addVertexToMesh(p1, n, color, bottomLeft));
        v2 = static_cast<unsigned int>(renderer->addVertexToMesh(p2, n, color, bottomRight));
        v3 = static_cast<unsigned int>(renderer->addVertexToMesh(p3, n, color, topRight));
        v4 = static_cast<unsigned int>(renderer->addVertexToMesh(p4, n, color, topLeft));
        renderer->addTriangleToMesh(v1, v2, v3);
        renderer->addTriangleToMesh(v1, v3, v4);
    }

    // Back
    n = -Vector3::unitZ();
    if (zNegative) {
        v5 = static_cast<unsigned int>(renderer->addVertexToMesh(p5, n, color, bottomLeft));
        v6 = static_cast<unsigned int>(renderer->addVertexToMesh(p6, n, color, bottomRight));
        v7 = static_cast<unsigned int>(renderer->addVertexToMesh(p7, n, color, topRight));
        v8 = static_cast<unsigned int>(renderer->addVertexToMesh(p8, n, color, topLeft));
        renderer->addTriangleToMesh(v5, v6, v7);
        renderer->addTriangleToMesh(v5, v7, v8);
    }

    // Right
    n = Vector3::unitX();
    if (xPositive) {
        v2 = static_cast<unsigned int>(renderer->addVertexToMesh(p2, n, color, bottomLeft));
        v5 = static_cast<unsigned int>(renderer->addVertexToMesh(p5, n, color, bottomRight));
        v8 = static_cast<unsigned int>(renderer->addVertexToMesh(p8, n, color, topRight));
        v3 = static_cast<unsigned int>(renderer->addVertexToMesh(p3, n, color, topLeft));
        renderer->addTriangleToMesh(v2, v5, v8);
        renderer->addTriangleToMesh(v2, v8, v3);
    }

    // Left
    n = -Vector3::unitX();
    if (xNegative) {
        v6 = static_cast<unsigned int>(renderer->addVertexToMesh(p6, n, color, bottomLeft));
        v1 = static_cast<unsigned int>(renderer->addVertexToMesh(p1, n, color, bottomRight));
        v4 = static_cast<unsigned int>(renderer->addVertexToMesh(p4, n, color, topRight));
        v7 = static_cast<unsigned int>(renderer->addVertexToMesh(p7, n, color, topLeft));
        renderer->addTriangleToMesh(v6, v1, v4);
        renderer->addTriangleToMesh(v6, v4, v7);
    }

    // Top
    n = Vector3::unitY();
    if (yPositive) {
        v4 = static_cast<unsigned int>(renderer->addVertexToMesh(p4, n, color, bottomLeft));
        v3 = static_cast<unsigned int>(renderer->addVertexToMesh(p3, n, color, bottomRight));
        v8 = static_cast<unsigned int>(renderer->addVertexToMesh(p8, n, color, topRight));
        v7 = static_cast<unsigned int>(renderer->addVertexToMesh(p7, n, color, topLeft));
        renderer->addTriangleToMesh(v4, v3, v8);
        renderer->addTriangleToMesh(v4, v8, v7);
    }

    // Bottom
    n = -Vector3::unitY();
    if (yNegative) {
        v6 = static_cast<unsigned int>(renderer->addVertexToMesh(p6, n, color, bottomLeft));
        v5 = static_cast<unsigned int>(renderer->addVertexToMesh(p5, n, color, bottomRight));
        v2 = static_cast<unsigned int>(renderer->addVertexToMesh(p2, n, color, topRight));
        v1 = static_cast<unsigned int>(renderer->addVertexToMesh(p1, n, color, topLeft));
        renderer->addTriangleToMesh(v6, v5, v2);
        renderer->addTriangleToMesh(v6, v2, v1);
    }
}

ChunkRenderer::ChunkRenderer() {
    _shader = new Shader("shader.vert", "shader.frag");
    _textureManager = TextureManager::getInstance();

    _shader->use();
}

void ChunkRenderer::setViewMatrix(const Matrix4& matrix) {
    _view = matrix;
    for (auto renderer: _meshRenderers) {
        renderer->setViewMatrix(_view);
    }
}

void ChunkRenderer::setProjectionMatrix(const Matrix4& matrix) {
    _projection = matrix;
    for (auto renderer: _meshRenderers) {
        renderer->setProjectionMatrix(_projection);
    }
}

bool ChunkRenderer::isEmpty(int meshId) {
    auto it = std::find(_meshIds.begin(), _meshIds.end(), meshId);
    if (it != _meshIds.end()) {
        int index = it - _meshIds.begin();
        return _meshRenderers[index]->isEmpty();
    }
    return true;
}

int ChunkRenderer::addChunk(const Vector3i& position, Block* blocks[][ConfigHelper::CHUNK_SIZE][ConfigHelper::CHUNK_SIZE]) {
    auto* renderer = new MeshRenderer(_shader);
    renderer->setViewMatrix(_view);
    renderer->setProjectionMatrix(_projection);
    renderer->setModelMatrix(
        Matrix4::createTranslation(
            static_cast<Vector3>(position) * ConfigHelper::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE));

    for (int x = 0; x < ConfigHelper::CHUNK_SIZE; x++) {
        for (int y = 0; y < ConfigHelper::CHUNK_SIZE; y++) {
            for (int z = 0; z < ConfigHelper::CHUNK_SIZE; z++) {
                if (!blocks[x][y][z]->isActive()) {
                    continue;
                }

                bool xNegative = true;
                if (x > 0) xNegative = !blocks[x - 1][y][z]->isActive();

                bool xPositive = true;
                if (x < ConfigHelper::CHUNK_SIZE - 1) xPositive = !blocks[x + 1][y][z]->isActive();

                bool yNegative = true;
                if (y > 0) yNegative = !blocks[x][y - 1][z]->isActive();

                bool yPositive = true;
                if (y < ConfigHelper::CHUNK_SIZE - 1) yPositive = !blocks[x][y + 1][z]->isActive();

                bool zNegative = true;
                if (z > 0) zNegative = !blocks[x][y][z - 1]->isActive();

                bool zPositive = true;
                if (z < ConfigHelper::CHUNK_SIZE - 1) zPositive = !blocks[x][y][z + 1]->isActive();

                createCube(xNegative, xPositive, yNegative, yPositive, zNegative, zPositive, x, y, z, blocks[x][y][z]->getColor(), blocks[x][y][z]->getTexturePos(), renderer);
            }
        }
    }

    return addChunk(renderer);
}

int ChunkRenderer::addChunk(MeshRenderer* renderer) {
    _meshRenderers.push_back(renderer);

    if (_meshIds.empty()) {
        _meshIds.push_back(1);
        return 1;
    }

    int max = *std::max_element(_meshIds.begin(), _meshIds.end()) + 1;

    _meshIds.push_back(max);

    return max;
}

MeshRenderer* ChunkRenderer::createChunkMeshToAdd(const Vector3i& position, Block* blocks[][ConfigHelper::CHUNK_SIZE][ConfigHelper::CHUNK_SIZE]) const {
    auto* renderer = new MeshRenderer(_shader);
    renderer->setViewMatrix(_view);
    renderer->setProjectionMatrix(_projection);
    renderer->setModelMatrix(
        Matrix4::createTranslation(
            static_cast<Vector3>(position) * ConfigHelper::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE));

    for (int x = 0; x < ConfigHelper::CHUNK_SIZE; x++) {
        for (int y = 0; y < ConfigHelper::CHUNK_SIZE; y++) {
            for (int z = 0; z < ConfigHelper::CHUNK_SIZE; z++) {
                if (!blocks[x][y][z]->isActive()) {
                    continue;
                }

                bool xNegative = true;
                if (x > 0) xNegative = !blocks[x - 1][y][z]->isActive();

                bool xPositive = true;
                if (x < ConfigHelper::CHUNK_SIZE - 1) xPositive = !blocks[x + 1][y][z]->isActive();

                bool yNegative = true;
                if (y > 0) yNegative = !blocks[x][y - 1][z]->isActive();

                bool yPositive = true;
                if (y < ConfigHelper::CHUNK_SIZE - 1) yPositive = !blocks[x][y + 1][z]->isActive();

                bool zNegative = true;
                if (z > 0) zNegative = !blocks[x][y][z - 1]->isActive();

                bool zPositive = true;
                if (z < ConfigHelper::CHUNK_SIZE - 1) zPositive = !blocks[x][y][z + 1]->isActive();

                createCube(xNegative, xPositive, yNegative, yPositive, zNegative, zPositive, x, y, z, blocks[x][y][z]->getColor(), blocks[x][y][z]->getTexturePos(), renderer);
            }
        }
    }

    return renderer;
}

void ChunkRenderer::removeChunk(int id) {
    auto it = std::find(_meshIds.begin(), _meshIds.end(), id);
    if (it != _meshIds.end()) {
        int index = it - _meshIds.begin();
        _meshIds.erase(it);
        delete _meshRenderers.at(index);
        _meshRenderers.erase(_meshRenderers.begin() + index);
    }
}

void ChunkRenderer::render(int meshId) {
    _textureManager->bindTexture();

    auto it = std::find(_meshIds.begin(), _meshIds.end(), meshId);
    if (it != _meshIds.end()) {
        int index = it - _meshIds.begin();
        _meshRenderers[index]->render();
    }
}

ChunkRenderer::~ChunkRenderer() {
    delete _shader;
    for (auto renderer: _meshRenderers) {
        delete renderer;
    }
}

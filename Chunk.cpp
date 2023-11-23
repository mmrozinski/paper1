#include "Chunk.h"

void Chunk::initialize() {
    for (auto&block: blocks) {
        for (auto&y: block) {
            for (auto&z: y) {
                z = new Block();
            }
        }
    }

    isInitialized = true;
}

void Chunk::generate() {
    for (auto&block: blocks) {
        for (auto&y: block) {
            for (auto&z: y) {
                z->active = true;
            }
        }
    }

    isGenerated = true;
}

void Chunk::updateFullSides() {
    fullSides.clear();

    for (auto side: allSides) {
        fullSides.insert(side);
    }

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            if (!blocks[x][y][0]->active) {
                fullSides.erase(SOUTH);
            }
            if (!blocks[x][y][CHUNK_SIZE - 1]->active) {
                fullSides.erase(NORTH);
            }
        }

        for (int z = 0; z < CHUNK_SIZE; z++) {
            if (!blocks[x][0][z]->active) {
                fullSides.erase(BOTTOM);
            }
            if (!blocks[x][CHUNK_SIZE - 1][z]->active) {
                fullSides.erase(TOP);
            }
        }
    }

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            if (!blocks[0][y][z]->active) {
                fullSides.erase(EAST);
            }
            if (!blocks[CHUNK_SIZE - 1][y][z]->active) {
                fullSides.erase(WEST);
            }
        }
    }
}

Vector3i Chunk::voxelToChunkPosition(Vector3 position) {
    Vector3 vec = ((position) / CHUNK_SIZE / Block::BLOCK_RENDER_SIZE);
    const int x = static_cast<int>(vec.x);
    const int y = static_cast<int>(vec.y);
    const int z = static_cast<int>(vec.z);

    return Vector3i(x, y, z);
}

Vector3 Chunk::chunkToVoxelPosiotion(Vector3i position) {
    return Vector3(static_cast<Vector3>(position) * CHUNK_SIZE * Block::BLOCK_RENDER_SIZE);
}

Chunk::Chunk(Vector3i position) {
    this->position = position;
}

void Chunk::load() {
    if (!isInitialized) {
        initialize();
    }
    isLoaded = true;
}

void Chunk::unload(ChunkRenderer* renderer) {
    if (meshId != -1)
    {
        renderer->removeChunk(meshId);
        meshId = -1;
    }

    isLoaded = false;
    isSetup = false;
    shouldRender = false;
}

void Chunk::setup(ChunkRenderer* renderer) {
    if (!isInitialized) {
        initialize();
    }
    if (!isGenerated) {
        generate();
    }
    meshId = renderer->addChunk(position, blocks);
    updateFullSides();
    isSetup = true;
}

void Chunk::rebuildMesh(ChunkRenderer* renderer) {
    if (meshId != -1) {
        renderer->removeChunk(meshId);
        meshId = -1;
    }
    meshId = renderer->addChunk(position, blocks);
    updateFullSides();
    needsRebuild = false;
}

void Chunk::updateRenderFlags(ChunkRenderer* renderer) {
    if (meshId != -1) {
        isEmpty = renderer->isEmpty(meshId);
    } else {
        isEmpty = true;
    }

    if (isEmpty || !isLoaded || !isSetup) {
        shouldRender = false;
    } else {
        if (isLoaded && isSetup) {
            shouldRender = true;
        }
    }
}

void Chunk::render(ChunkRenderer* renderer) {
    if (meshId != -1) {
        renderer->render(meshId);
    }
}

Chunk::~Chunk() {
    for (auto&block: blocks) {
        for (auto&y: block) {
            for (auto&z: y) {
                delete z;
            }
        }
    }
}

#include "Chunk.h"

#include "BlockGrass.h"
#include "BlockStone.h"
#include "BlockWater.h"

void Chunk::initialize() {
    for (auto&block: blocks) {
        for (auto&y: block) {
            for (auto&z: y) {
                z = new BlockStone();
            }
        }
    }

    isInitialized = true;
}

void Chunk::generate(const FastNoiseLite& noise) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            Vector3 globalPosition = chunkToVoxelPosition(position);

            float height = noise.GetNoise(globalPosition.x + x, globalPosition.z + z) * CHUNK_SIZE * 8;

            for (int y = 0; y < CHUNK_SIZE; y++) {
                if (globalPosition.y + y < 0) {
                    delete blocks[x][y][z];
                    blocks[x][y][z] = new BlockWater();
                    blocks[x][y][z]->setActive(true);
                }

                if (globalPosition.y + y > height) {
                    continue;
                }

                if (globalPosition.y + y > height - 3 && globalPosition.y + y > 0) {
                    delete blocks[x][y][z];
                    blocks[x][y][z] = new BlockGrass();
                } else {
                    delete blocks[x][y][z];
                    blocks[x][y][z] = new BlockStone();
                }
                blocks[x][y][z]->setActive(true);
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
            if (!blocks[x][y][0]->isActive()) {
                fullSides.erase(SOUTH);
            }
            if (!blocks[x][y][CHUNK_SIZE - 1]->isActive()) {
                fullSides.erase(NORTH);
            }
        }

        for (int z = 0; z < CHUNK_SIZE; z++) {
            if (!blocks[x][0][z]->isActive()) {
                fullSides.erase(BOTTOM);
            }
            if (!blocks[x][CHUNK_SIZE - 1][z]->isActive()) {
                fullSides.erase(TOP);
            }
        }
    }

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            if (!blocks[0][y][z]->isActive()) {
                fullSides.erase(EAST);
            }
            if (!blocks[CHUNK_SIZE - 1][y][z]->isActive()) {
                fullSides.erase(WEST);
            }
        }
    }
}

Vector3i Chunk::voxelToChunkPosition(const Vector3& position) {
    Vector3 vec = ((position) / CHUNK_SIZE / Block::BLOCK_RENDER_SIZE);
    const int x = static_cast<int>(vec.x);
    const int y = static_cast<int>(vec.y);
    const int z = static_cast<int>(vec.z);

    return {x, y, z};
}

Vector3 Chunk::chunkToVoxelPosition(Vector3i position) {
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

void Chunk::setup(const FastNoiseLite& noise) {
    if (!isInitialized) {
        initialize();
    }
    if (!isGenerated) {
        generate(noise);
    }
    updateFullSides();
    isSetup = true;
    needsRebuild = true;
}

void Chunk::rebuildMesh(ChunkRenderer* renderer) {
    if (meshId != -1) {
        renderer->removeChunk(meshId);
    }

    if (rebuiltChunkRenderer == nullptr) {
        createMeshForRebuild(renderer);
    }

    meshId = renderer->addChunk(rebuiltChunkRenderer);
    updateFullSides();
    needsRebuild = false;
}

void Chunk::createMeshForRebuild(const ChunkRenderer* renderer) {
    rebuiltChunkRenderer = renderer->createChunkMeshToAdd(position, blocks);
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

void Chunk::render(ChunkRenderer* renderer) const {
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

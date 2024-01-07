#include "ChunkManager.h"

#include "BlockStone.h"
#include "RayCaster.h"

Chunk* ChunkManager::getChunk(const Vector3i&position) const {
    for (const auto chunk: masterList) {
        if (chunk->position == position) {
            return chunk;
        }
    }

    return nullptr;
}

void ChunkManager::prepareFlowLists(Chunk* chunk, const Vector3&position) {
    const Vector3i chunkPosition = Chunk::worldToChunkPosition(position);
    const Vector3i distance = chunk->position - chunkPosition;

    int xDist = std::abs(distance.x);
    int yDist = std::abs(distance.y);
    int zDist = std::abs(distance.z);

    if (!chunk->isLoaded && xDist <= ConfigHelper::CHUNK_VISIBILITY_DISTANCE && yDist <= ConfigHelper::CHUNK_VISIBILITY_DISTANCE && zDist <=
        ConfigHelper::CHUNK_VISIBILITY_DISTANCE) {
        loadList.insert(chunk);
    }

    if (!chunk->isSetup) {
        setupList.insert(chunk);
    }

    if (chunk->needsRebuild) {
        rebuildList.insert(chunk);
    }

    if (xDist > ConfigHelper::CHUNK_VISIBILITY_DISTANCE || yDist > ConfigHelper::CHUNK_VISIBILITY_DISTANCE || zDist >
        ConfigHelper::CHUNK_VISIBILITY_DISTANCE) {
        unloadList.insert(chunk);
    }

    if (chunk->isLoaded && chunk->isSetup && !chunk->shouldRender) {
        flagsUpdateList.insert(chunk);
    }

    visibleList.insert(chunk);
}

void ChunkManager::populateMasterList(const Vector3&position, const int x, const int y, const int z) {
    const Vector3i chunkPosition = Chunk::worldToChunkPosition(position) + Vector3i(x, y, z);

    if (Chunk* chunk = getChunk(chunkPosition); chunk == nullptr) {
        // if (chunkPosition.x >= 0 && chunkPosition.y >= 0 && chunkPosition.z >= 0 && chunkPosition.x <
        //     WORLD_SIZE && chunkPosition.y < WORLD_SIZE && chunkPosition.z < WORLD_SIZE) {
        //     chunk = new Chunk(chunkPosition);
        //     masterList.insert(chunk);
        // }
        chunk = new Chunk(chunkPosition);
        masterList.insert(chunk);
    }
}

void ChunkManager::optimizeForNeighbour(Chunk* chunk) const {
    chunk->updateRenderFlags(_renderer);

    if (chunk->shouldRender) {
        Chunk* neighbour = nullptr;
        bool isSurrounded = true;

        neighbour = getChunk(chunk->position + Vector3i(-1, 0, 0));
        if (neighbour != nullptr && isSurrounded) {
            if (!neighbour->fullSides.contains(Chunk::Sides::WEST)) {
                isSurrounded = false;
            }
        }
        else {
            isSurrounded = false;
        }

        neighbour = getChunk(chunk->position + Vector3i(1, 0, 0));
        if (neighbour != nullptr && isSurrounded) {
            if (!neighbour->fullSides.contains(Chunk::Sides::EAST)) {
                isSurrounded = false;
            }
        }
        else {
            isSurrounded = false;
        }

        neighbour = getChunk(chunk->position + Vector3i(0, -1, 0));
        if (neighbour != nullptr && isSurrounded) {
            if (!neighbour->fullSides.contains(Chunk::Sides::TOP)) {
                isSurrounded = false;
            }
        }
        else {
            isSurrounded = false;
        }

        neighbour = getChunk(chunk->position + Vector3i(0, 1, 0));
        if (neighbour != nullptr && isSurrounded) {
            if (!neighbour->fullSides.contains(Chunk::Sides::BOTTOM)) {
                isSurrounded = false;
            }
        }
        else {
            isSurrounded = false;
        }

        neighbour = getChunk(chunk->position + Vector3i(0, 0, -1));
        if (neighbour != nullptr && isSurrounded) {
            if (!neighbour->fullSides.contains(Chunk::Sides::NORTH)) {
                isSurrounded = false;
            }
        }
        else {
            isSurrounded = false;
        }

        neighbour = getChunk(chunk->position + Vector3i(0, 0, 1));
        if (neighbour != nullptr && isSurrounded) {
            if (!neighbour->fullSides.contains(Chunk::Sides::SOUTH)) {
                isSurrounded = false;
            }
        }
        else {
            isSurrounded = false;
        }

        if (isSurrounded) {
            chunk->shouldRender = false;
        }
    }
}

ChunkManager::ChunkManager() {
    _renderer = new ChunkRenderer();
    _meshRenderer = new MeshRenderer();

    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetSeed(static_cast<int>(time(nullptr)));
}

void ChunkManager::updateLoadList() {
    int chunksLoaded = 0;
    std::vector<std::future<void>> loadThreads;
    for (auto chunk: loadList) {
        if (chunksLoaded >= ConfigHelper::CHUNKS_ASYNC_LIMIT_PER_FRAME) {
            break;
        }

        if (!chunk->isLoaded) {
            loadThreads.push_back(std::async(std::launch::async, &Chunk::load, chunk));
            chunksLoaded++;
        }
    }

    for (auto&thread: loadThreads) {
        thread.wait();
    }
    loadList.clear();
}

void ChunkManager::updateSetupList() {
    std::vector<std::future<void>> setupThreads;
    for (auto chunk: setupList) {
        if (chunk->isLoaded && !chunk->isSetup) {
            setupThreads.push_back(std::async(std::launch::async, &Chunk::setup, chunk, noise));
        }
    }

    for (auto&thread: setupThreads) {
        thread.wait();
    }
    setupList.clear();
}

void ChunkManager::updateRebuildList() {
    int chunksRebuilt = 0;
    std::vector<std::future<void>> rebuildThreads;

    for (auto chunk: rebuildList) {
        if (chunk->isLoaded && chunk->isSetup) {
            if (chunksRebuilt >= ConfigHelper::CHUNKS_ASYNC_LIMIT_PER_FRAME) {
                break;
            }

            rebuildThreads.push_back(std::async(std::launch::async, &Chunk::createMeshForRebuild, chunk, _renderer));

            flagsUpdateList.insert(chunk);

            Chunk* neighbour = nullptr;

            neighbour = getChunk(chunk->position + Vector3i(-1, 0, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.insert(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(1, 0, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.insert(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, -1, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.insert(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, 1, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.insert(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, 0, -1));
            if (neighbour != nullptr) {
                flagsUpdateList.insert(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, 0, 1));
            if (neighbour != nullptr) {
                flagsUpdateList.insert(neighbour);
            }

            chunksRebuilt++;
        }
    }

    for (auto&thread: rebuildThreads) {
        thread.wait();
    }

    for (auto chunk: rebuildList) {
        if (chunk->isLoaded && chunk->isSetup) {
            chunk->rebuildMesh(_renderer);
        }
    }

    rebuildList.clear();
}

void ChunkManager::updateFlagsList() {
    std::vector<std::future<void>> updateFlagsThreads;

    for (const auto chunk: flagsUpdateList) {
        //updateFlagsThreads.push_back(std::async(std::launch::async, optimizeForNeighbour, this, chunk));
        optimizeForNeighbour(chunk);
    }

    for (auto&thread: updateFlagsThreads) {
        thread.wait();
    }

    flagsUpdateList.clear();
}

void ChunkManager::updateUnloadList() {
    for (const auto chunk: unloadList) {
        if (chunk->isLoaded) {
            chunk->unload(_renderer);
        }

        masterList.erase(chunk);

        delete chunk;
    }
    unloadList.clear();
}

void ChunkManager::updateVisible(const Vector3&position) {
    visibleList.clear();

    std::vector<std::future<void>> listUpdateThreads;
    std::vector<std::future<void>> populateMasterListThreads;

    if (const Vector3i currentPosition = Chunk::worldToChunkPosition(position); lastPosition != currentPosition) {
        lastPosition = currentPosition;
        for (int x = -ConfigHelper::CHUNK_VISIBILITY_DISTANCE; x < ConfigHelper::CHUNK_VISIBILITY_DISTANCE; x++) {
            for (int y = -ConfigHelper::CHUNK_VISIBILITY_DISTANCE; y < ConfigHelper::CHUNK_VISIBILITY_DISTANCE; y++) {
                for (int z = -ConfigHelper::CHUNK_VISIBILITY_DISTANCE; z < ConfigHelper::CHUNK_VISIBILITY_DISTANCE; z++) {
                    //populateMasterListThreads.push_back(std::async(std::launch::async, populateMasterList, this, position, x, y, z));
                    populateMasterList(position, x, y, z);
                }
            }
        }
    }

    for (auto&thread: populateMasterListThreads) {
        thread.wait();
    }

    for (const auto chunk: masterList) {
        //listUpdateThreads.push_back(std::async(std::launch::async, &prepareFlowLists, this, chunk, position));
        prepareFlowLists(chunk, position);
    }

    for (auto&thread: listUpdateThreads) {
        thread.wait();
    }
}

void ChunkManager::updateRenderList(const Camera&camera) {
    Matrix4 projection = camera.getProjectionMatrix();
    Matrix4 view = camera.getViewMatrix();
    frustum = Frustum(view * projection);

    renderList.clear();

    for (auto chunk: visibleList) {
        if (chunk->isLoaded && chunk->isSetup) {
            if (chunk->shouldRender) {
                float offset = (ConfigHelper::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE) / 2.0f;
                Vector3 chunkCenter = Chunk::chunkToWorldPosition(chunk->position) + Vector3(offset, offset, offset);
                float size = (ConfigHelper::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE) / 2.0f;
                if (frustum.cubeInFrustum(chunkCenter, size, size, size) != Frustum::FRUSTUM_OUTSIDE) {
                    renderList.insert(chunk);
                }
            }
        }
    }
}

void ChunkManager::updateLists(const Camera&camera, bool doTickUpdates) {
    if (doTickUpdates) {
        updateLoadList();
        updateSetupList();
        updateRebuildList();
        updateFlagsList();
        updateUnloadList();
        updateVisible(camera.getPosition());
    }

    updateRenderList(camera);
}

void ChunkManager::update(const Camera&camera) {
    updateLists(camera, true);

    _renderer->setViewMatrix(camera.getViewMatrix());
    _renderer->setProjectionMatrix(camera.getProjectionMatrix());
}

void ChunkManager::update(const Camera&camera, bool doListUpdates, bool doTickUpdates) {
    if (doListUpdates) {
        updateLists(camera, doTickUpdates);

        frustum.preRender(_meshRenderer);
    }

    _renderer->setViewMatrix(camera.getViewMatrix());
    _renderer->setProjectionMatrix(camera.getProjectionMatrix());

    _meshRenderer->setViewMatrix(camera.getViewMatrix());
    _meshRenderer->setProjectionMatrix(camera.getProjectionMatrix());
    _meshRenderer->setModelMatrix(Matrix4::createTranslation(Vector3()));
}

void ChunkManager::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto chunk: renderList) {
        chunk->render(_renderer);
    }
    //_meshRenderer->render();
    glutSwapBuffers();
}

void ChunkManager::breakBlock(const Camera&camera) {
    RayCaster::BlockRay lookRay = RayCaster::castRay(camera.getPosition(), camera.getPosition() + (camera.getFront() * ConfigHelper::BREAK_RANGE), 100);
    for (auto intersection: lookRay.getIntersections()) {
        Vector3i chunkPosition = Chunk::worldToChunkPosition(Vector3(intersection) + (Block::BLOCK_RENDER_SIZE / 2.0f));

        if (Chunk* chunk = getChunk(chunkPosition); chunk != nullptr && chunk->isLoaded) {
            if (const Vector3i blockLocalPos = Chunk::worldPositionToLocalBlockOffset(intersection);
                chunk->blocks[blockLocalPos.x][blockLocalPos.y][blockLocalPos.z]->isActive()) {
                chunk->breakBlock(blockLocalPos);
                std::cout << "Broken block at: " << intersection.x << " " << intersection.y << " " << intersection.z <<
                        std::endl;
                std::cout << "In chunk: " << chunkPosition.x << " " << chunkPosition.y << " " << chunkPosition.z <<
                        std::endl;
                std::cout << "Locally: " << blockLocalPos.x << " " << blockLocalPos.y << " " << blockLocalPos.z <<
                        std::endl;

                break;
            }
        }
    }
}

void ChunkManager::placeBlock(const Camera& camera) {
    RayCaster::BlockRay lookRay = RayCaster::castRay(camera.getPosition(), camera.getPosition() + (camera.getFront() * ConfigHelper::BREAK_RANGE), 100);
    auto intersections = lookRay.getIntersections();
    for (int i = 0; i < intersections.size(); i++) {
        auto intersection = intersections[i];
        Vector3i chunkPosition = Chunk::worldToChunkPosition(Vector3(intersection) + (Block::BLOCK_RENDER_SIZE / 2.0f));

        if (Chunk* chunk = getChunk(chunkPosition); chunk != nullptr && chunk->isLoaded) {
            if (const Vector3i blockLocalPos = Chunk::worldPositionToLocalBlockOffset(intersection);
                chunk->blocks[blockLocalPos.x][blockLocalPos.y][blockLocalPos.z]->isActive()) {
                if (i == 0) {
                    break;
                }
                intersection = intersections[i - 1];

                Vector3i placeChunkPosition = Chunk::worldToChunkPosition(Vector3(intersection) + (Block::BLOCK_RENDER_SIZE / 2.0f));

                if (Chunk* placeChunk = getChunk(placeChunkPosition); placeChunk != nullptr && placeChunk->isLoaded) {
                    BlockStone placedBlock;
                    placedBlock.setActive(true);
                    placeChunk->setBlock(Chunk::worldPositionToLocalBlockOffset(intersection), placedBlock);
                    std::cout << "Placed block at: " << intersection.x << " " << intersection.y << " " << intersection.z <<
                            std::endl;
                    std::cout << "In chunk: " << placeChunkPosition.x << " " << placeChunkPosition.y << " " << placeChunkPosition.z <<
                            std::endl;
                    std::cout << "Locally: " << blockLocalPos.x << " " << blockLocalPos.y << " " << blockLocalPos.z <<
                            std::endl;

                    break;
                }
            }
        }
    }
}

ChunkManager::~ChunkManager() {
    for (auto chunk: masterList) {
        delete chunk;
    }
    delete _renderer;
    delete _meshRenderer;
}

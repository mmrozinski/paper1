#include "ChunkManager.h"

Chunk* ChunkManager::getChunk(const Vector3i& position) const {
    for (const auto chunk: masterList) {
        if (chunk->position == position) {
            return chunk;
        }
    }

    return nullptr;
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
    for (auto chunk: loadList) {
        if (chunksLoaded >= CHUNKS_ASYNC_LIMIT_PER_FRAME) {
            break;
        }

        if (!chunk->isLoaded) {
            // std::thread loadThread(&Chunk::load, chunk);
            // loadThread.detach();
            chunk->load();
            chunksLoaded++;
        }
    }
    loadList.clear();
}

void ChunkManager::updateSetupList() {
    std::vector<std::thread> setupThreads;
    for (auto chunk: setupList) {
        if (chunk->isLoaded && !chunk->isSetup) {
            setupThreads.emplace_back(&Chunk::setup, chunk, noise);
        }
        for (auto& thread: setupThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
    setupList.clear();
}

void ChunkManager::updateRebuildList() {
    int chunksRebuilt = 0;
    std::vector<std::thread> rebuildThreads;
    for (auto chunk: rebuildList) {
        if (chunk->isLoaded && chunk->isSetup) {
            if (chunksRebuilt >= CHUNKS_ASYNC_LIMIT_PER_FRAME) {
                break;
            }

            //rebuildThreads.emplace_back(&Chunk::createMeshForRebuild, chunk, _renderer);
            chunk->createMeshForRebuild(_renderer);

            flagsUpdateList.push_back(chunk);

            Chunk* neighbour = nullptr;

            neighbour = getChunk(chunk->position + Vector3i(-1, 0, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.push_back(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(1, 0, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.push_back(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, -1, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.push_back(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, 1, 0));
            if (neighbour != nullptr) {
                flagsUpdateList.push_back(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, 0, -1));
            if (neighbour != nullptr) {
                flagsUpdateList.push_back(neighbour);
            }

            neighbour = getChunk(chunk->position + Vector3i(0, 0, 1));
            if (neighbour != nullptr) {
                flagsUpdateList.push_back(neighbour);
            }

            chunksRebuilt++;
        }
    }

    // for (auto& thread: rebuildThreads) {
    //     thread.join();
    // }

    for (auto chunk: rebuildList) {
        if (chunk->isLoaded && chunk->isSetup) {
            chunk->rebuildMesh(_renderer);
        }
    }

    rebuildList.clear();
}

void ChunkManager::updateFlagsList() {
    for (auto chunk: flagsUpdateList) {
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
        flagsUpdateList.clear();
    }
}

void ChunkManager::updateUnloadList() {
    for (const auto chunk: unloadList) {
        if (chunk->isLoaded) {
            chunk->unload(_renderer);
        }

        masterList.erase(std::remove(masterList.begin(), masterList.end(), chunk), masterList.end());
        // loadList.erase(std::remove(loadList.begin(), loadList.end(), chunk), loadList.end());
        // setupList.erase(std::remove(setupList.begin(), setupList.end(), chunk), setupList.end());
        // rebuildList.erase(std::remove(rebuildList.begin(), rebuildList.end(), chunk), rebuildList.end());
        // flagsUpdateList.erase(std::remove(flagsUpdateList.begin(), flagsUpdateList.end(), chunk), flagsUpdateList.end());
        // unloadList.erase(std::remove(unloadList.begin(), unloadList.end(), chunk), unloadList.end());
        // visibleList.erase(std::remove(visibleList.begin(), visibleList.end(), chunk), visibleList.end());
        // renderList.erase(std::remove(renderList.begin(), renderList.end(), chunk), renderList.end());
        //
        // delete chunk;
    }
    unloadList.clear();
}

void ChunkManager::updateVisible(const Vector3& position) {
    visibleList.clear();

    for (int x = -CHUNK_VISIBILITY_DISTANCE; x < CHUNK_VISIBILITY_DISTANCE; x++) {
        for (int y = -CHUNK_VISIBILITY_DISTANCE; y < CHUNK_VISIBILITY_DISTANCE; y++) {
            for (int z = -CHUNK_VISIBILITY_DISTANCE; z < CHUNK_VISIBILITY_DISTANCE; z++) {
                Vector3i chunkPosition = Chunk::voxelToChunkPosition(position) + Vector3i(x, y, z);
                Chunk* chunk = getChunk(chunkPosition);

                if (chunk == nullptr) {
                    // if (chunkPosition.x >= 0 && chunkPosition.y >= 0 && chunkPosition.z >= 0 && chunkPosition.x <
                    //     WORLD_SIZE && chunkPosition.y < WORLD_SIZE && chunkPosition.z < WORLD_SIZE) {
                    //     chunk = new Chunk(chunkPosition);
                    //     masterList.push_back(chunk);
                    // }
                    chunk = new Chunk(chunkPosition);
                    masterList.push_back(chunk);
                }
            }
        }
    }

    for (auto chunk: masterList) {
        Vector3i chunkPosition = Chunk::voxelToChunkPosition(position);
        Vector3i distance = chunk->position - chunkPosition;

        int xDist = std::abs(distance.x);
        int yDist = std::abs(distance.y);
        int zDist = std::abs(distance.z);

        if (!chunk->isLoaded && xDist <= CHUNK_VISIBILITY_DISTANCE && yDist <= CHUNK_VISIBILITY_DISTANCE && zDist <=
            CHUNK_VISIBILITY_DISTANCE) {
            loadList.push_back(chunk);
        }

        if (!chunk->isSetup) {
            setupList.push_back(chunk);
        }

        if (chunk->needsRebuild) {
            rebuildList.push_back(chunk);
        }

        if (xDist > CHUNK_VISIBILITY_DISTANCE || yDist > CHUNK_VISIBILITY_DISTANCE || zDist >
            CHUNK_VISIBILITY_DISTANCE) {
            unloadList.push_back(chunk);
        }

        if (chunk->isLoaded && chunk->isSetup && !chunk->shouldRender) {
            flagsUpdateList.push_back(chunk);
        }

        visibleList.push_back(chunk);
    }
}

void ChunkManager::updateRenderList(const Camera& camera) {
    Matrix4 projection = camera.getProjectionMatrix();
    Matrix4 view = camera.getViewMatrix();
    frustum = Frustum(view * projection);

    renderList.clear();

    for (auto chunk: visibleList) {
        if (chunk->isLoaded && chunk->isSetup) {
            if (chunk->shouldRender) {
                float offset = (Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE) / 2.0f;
                Vector3 chunkCenter = Chunk::chunkToVoxelPosition(chunk->position) + Vector3(offset, offset, offset);
                float size = (Chunk::CHUNK_SIZE * Block::BLOCK_RENDER_SIZE) / 2.0f;
                if (frustum.cubeInFrustum(chunkCenter, size, size, size) != Frustum::FRUSTUM_OUTSIDE) {
                    renderList.push_back(chunk);
                }
            }
        }
    }
}

void ChunkManager::updateLists(const Camera& camera) {
    updateLoadList();
    updateSetupList();
    updateRebuildList();
    updateFlagsList();
    updateUnloadList();
    updateVisible(camera.getPosition());

    updateRenderList(camera);
}

void ChunkManager::update(Camera camera) {
    updateLists(camera);

    _renderer->setViewMatrix(camera.getViewMatrix());
    _renderer->setProjectionMatrix(camera.getProjectionMatrix());
}

void ChunkManager::update(const Camera& camera, bool doListUpdates) {
    if (doListUpdates) {
        updateLists(camera);

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

ChunkManager::~ChunkManager() {
    for (auto chunk: masterList) {
        delete chunk;
    }
    delete _renderer;
    delete _meshRenderer;
}

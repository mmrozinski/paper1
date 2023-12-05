#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut_std.h>

#include "Camera.h"
#include "Chunk.h"
#include "Frustum.h"
#include "FastNoiseLite.h"


class ChunkManager {
private:
    const int CHUNKS_ASYNC_LIMIT_PER_FRAME = 16;
    const int CHUNK_VISIBILITY_DISTANCE = 8;

    const int WORLD_SIZE = 16;
    std::vector<Chunk*> masterList = std::vector<Chunk*>();

    std::vector<Chunk*> loadList = std::vector<Chunk*>();
    std::vector<Chunk*> setupList = std::vector<Chunk*>();
    std::vector<Chunk*> rebuildList = std::vector<Chunk*>();
    std::vector<Chunk*> flagsUpdateList = std::vector<Chunk*>();
    std::vector<Chunk*> unloadList = std::vector<Chunk*>();
    std::vector<Chunk*> visibleList = std::vector<Chunk*>();
    std::vector<Chunk*> renderList = std::vector<Chunk*>();

    ChunkRenderer* _renderer;
    MeshRenderer* _meshRenderer;

    Frustum frustum;
    FastNoiseLite noise;

    Chunk* getChunk(Vector3i position);
public:
    ChunkManager();

    void updateLoadList();
    void updateSetupList();
    void updateRebuildList();
    void updateFlagsList();
    void updateUnloadList();
    void updateVisible(Vector3 position);
    void updateRenderList(Camera camera);
    void update(Camera camera);
    void update(Camera camera, bool doListUpdates);
    void render();

    ~ChunkManager();
};



#endif //CHUNKMANAGER_H

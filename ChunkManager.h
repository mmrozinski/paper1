#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <thread>
#include <future>

#include "Camera.h"
#include "Chunk.h"
#include "Frustum.h"
#include "FastNoiseLite.h"


class ChunkManager {
private:
    const int CHUNKS_ASYNC_LIMIT_PER_FRAME = 1024;
    const int CHUNK_VISIBILITY_DISTANCE = 8;

    const int WORLD_SIZE = 1;

    const float BREAK_RANGE = 5.0f;
    std::set<Chunk*> masterList = std::set<Chunk*>();

    std::set<Chunk*> loadList = std::set<Chunk*>();
    std::set<Chunk*> setupList = std::set<Chunk*>();
    std::set<Chunk*> rebuildList = std::set<Chunk*>();
    std::set<Chunk*> flagsUpdateList = std::set<Chunk*>();
    std::set<Chunk*> unloadList = std::set<Chunk*>();
    std::set<Chunk*> visibleList = std::set<Chunk*>();
    std::set<Chunk*> renderList = std::set<Chunk*>();

    ChunkRenderer* _renderer;
    MeshRenderer* _meshRenderer;

    Frustum frustum;
    FastNoiseLite noise;

    Vector3i lastPosition = Vector3i(99, 99, 99);

    Chunk* getChunk(const Vector3i& position) const;

    void prepareFlowLists(Chunk* chunk, const Vector3& position);
    void populateMasterList(const Vector3& position, int x, int y, int z);
    void optimizeForNeighbour(Chunk* chunk) const;
public:
    ChunkManager();

    void updateLoadList();
    void updateSetupList();
    void updateRebuildList();
    void updateFlagsList();
    void updateUnloadList();
    void updateVisible(const Vector3& position);
    void updateRenderList(const Camera& camera);
    void updateLists(const Camera& camera, bool doTickUpdates);
    void update(const Camera& camera);
    void update(const Camera& camera, bool doListUpdates, bool doTickUpdates);
    void render();

    void breakBlock(const Camera& camera);
    void placeBlock(const Camera& camera);

    ~ChunkManager();
};



#endif //CHUNKMANAGER_H

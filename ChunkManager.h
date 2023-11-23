#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H
#include <vector>

#include "Camera.h"
#include "Chunk.h"
#include <GL/freeglut_std.h>

class ChunkManager {
private:
    const int CHUNKS_ASYNC_LIMIT_PER_FRAME = 8;
    const int CHUNK_VISIBILITY_DISTANCE = 4;

    const int WORLD_SIZE = 8;
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

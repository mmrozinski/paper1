#ifndef CHUNK_H
#define CHUNK_H
#include <set>

#include "Block.h"
#include "ChunkRenderer.h"
#include "FastNoiseLite.h"
#include "Vector3.h"
#include "Vector3i.h"

class Chunk {
private:
    bool isInitialized = false;
    bool isGenerated = false;

    int meshId = -1;

    void initialize();
    void generate(FastNoiseLite noise);
    void updateFullSides();

public:
    static constexpr int CHUNK_SIZE = 8;

    enum Sides { NORTH, SOUTH, WEST, EAST, TOP, BOTTOM };
    static constexpr Sides allSides[] = { NORTH, SOUTH, WEST, EAST, TOP, BOTTOM };

    Block* blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    Vector3i position;

    bool isLoaded = false;
    bool isSetup = false;
    bool shouldRender = false;
    bool isEmpty = false;
    bool needsRebuild = true;

    std::set<Sides> fullSides = std::set<Sides>();

    static Vector3i voxelToChunkPosition(Vector3 position);
    static Vector3 chunkToVoxelPosition(Vector3i position);

    Chunk(Vector3i position);

    void load();
    void unload(ChunkRenderer* renderer);
    void setup(ChunkRenderer* renderer, FastNoiseLite noise);
    void rebuildMesh(ChunkRenderer* renderer);
    void updateRenderFlags(ChunkRenderer* renderer);
    void render(ChunkRenderer* renderer);

    ~Chunk();
};


#endif //CHUNK_H

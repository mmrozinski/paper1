#ifndef CHUNK_H
#define CHUNK_H
#include <set>

#include "Block.h"
#include "ChunkRenderer.h"
#include "extern/FastNoiseLite.h"
#include "Vector3.h"
#include "Vector3i.h"

class Chunk {
private:
    bool isInitialized = false;
    bool isGenerated = false;

    MeshRenderer* rebuiltChunkRenderer = nullptr;
    int meshId = -1;

    void initialize();
    void generate(const FastNoiseLite& noise);
    void updateFullSides();

public:
    enum Sides { NORTH, SOUTH, WEST, EAST, TOP, BOTTOM };
    static constexpr Sides allSides[] = { NORTH, SOUTH, WEST, EAST, TOP, BOTTOM };

    Block* blocks[ConfigHelper::CHUNK_SIZE][ConfigHelper::CHUNK_SIZE][ConfigHelper::CHUNK_SIZE]{};

    Vector3i position;

    bool isLoaded = false;
    bool isSetup = false;
    bool shouldRender = false;
    bool isEmpty = false;
    bool needsRebuild = true;

    std::set<Sides> fullSides = std::set<Sides>();

    static Vector3i worldToChunkPosition(const Vector3& position);
    static Vector3 chunkToWorldPosition(const Vector3i& position);
    static Vector3i worldToBlockPosition(const Vector3& position);
    static Vector3i worldPositionToLocalBlockOffset(const Vector3& position);

    explicit Chunk(Vector3i position);

    void load();
    void unload(ChunkRenderer* renderer);
    void setup(const FastNoiseLite& noise);
    void rebuildMesh(ChunkRenderer* renderer);
    void createMeshForRebuild(const ChunkRenderer* renderer);
    void updateRenderFlags(ChunkRenderer* renderer);
    void render(ChunkRenderer* renderer) const;

    void breakBlock(const Vector3i& position);
    void setBlock(const Vector3i& position, const Block& block);

    ~Chunk();
};


#endif //CHUNK_H

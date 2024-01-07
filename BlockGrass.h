#ifndef BLOCKGRASS_H
#define BLOCKGRASS_H
#include "Block.h"


class BlockGrass: public Block {
public:
    Vector3 getColor() const override {
        return Vector3(0.1f, 0.6f, 0.1f);
    }

    [[nodiscard]] Block* clone() const override {
        Block* cloned = new BlockGrass();
        cloned->setActive(this->isActive());
        return cloned;
    }

    Vector3i getTexturePos() const override {
        return Vector3i(0, 0, 0);
    }
};



#endif //BLOCKGRASS_H

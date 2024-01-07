#ifndef BLOCKWATER_H
#define BLOCKWATER_H
#include "Block.h"
#include "Vector3.h"


class BlockWater: public Block {
    Vector3 getColor() const override {
        return Vector3(0.1f, 0.2f, 0.7f);
    }

public:
    [[nodiscard]] Block* clone() const override {
        Block* cloned = new BlockWater();
        cloned->setActive(this->isActive());
        return cloned;
    }

    Vector3i getTexturePos() const override {
        return Vector3i(0, 1, 0);
    }
};



#endif //BLOCKWATER_H

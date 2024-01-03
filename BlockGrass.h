#ifndef BLOCKGRASS_H
#define BLOCKGRASS_H
#include "Block.h"


class BlockGrass: public Block {
    Vector3 getColor() const override {
        return Vector3(0.1f, 0.7f, 0.1f);
    }

public:
    [[nodiscard]] Block* clone() const override {
        Block* cloned = new BlockGrass();
        cloned->setActive(this->isActive());
        return cloned;
    }
};



#endif //BLOCKGRASS_H

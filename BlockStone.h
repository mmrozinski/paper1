#ifndef BLOCKSTONE_H
#define BLOCKSTONE_H
#include "Block.h"



class BlockStone: public Block {
    Vector3 getColor() const override {
        return Vector3(0.4f, 0.4f, 0.4f);
    }

public:
    [[nodiscard]] Block* clone() const override {
        Block* cloned = new BlockStone();
        cloned->setActive(this->isActive());
        return cloned;
    }

    Vector3i getTexturePos() const override {
        return Vector3i(1, 0, 0);
    }
};



#endif //BLOCKSTONE_H

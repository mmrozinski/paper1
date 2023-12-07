#ifndef BLOCKWATER_H
#define BLOCKWATER_H
#include "Block.h"
#include "Vector3.h"


class BlockWater: public Block {
    Vector3 getColor() const override {
        return Vector3(0.1f, 0.2f, 0.6f);
    }
};



#endif //BLOCKWATER_H

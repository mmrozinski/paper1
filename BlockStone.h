#ifndef BLOCKSTONE_H
#define BLOCKSTONE_H
#include "Block.h"



class BlockStone: public Block {
    Vector3 getColor() const override {
        return Vector3(0.4f, 0.4f, 0.4f);
    }
};



#endif //BLOCKSTONE_H

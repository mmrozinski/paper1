#ifndef BLOCK_H
#define BLOCK_H
#include "Vector3.h"


class Block {
public:
    static constexpr float BLOCK_RENDER_SIZE = 1.0f;
    bool active = false;
    virtual Vector3 getColor() const = 0;
};



#endif //BLOCK_H

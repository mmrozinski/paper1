#ifndef BLOCK_H
#define BLOCK_H
#include "Vector3.h"


class Block {
private:
    bool active = false;
public:
    static constexpr float BLOCK_RENDER_SIZE = 1.0f;

    virtual Vector3 getColor() const = 0;
    virtual Vector3i getTexturePos() const = 0;

    bool isActive() const {
        return active;
    }

    void setActive(const bool active) {
        this->active = active;
    }

    [[nodiscard]] virtual Block* clone() const = 0;
};



#endif //BLOCK_H

#include "RayCaster.h"

#include "Chunk.h"

void RayCaster::BlockRay::addIntersection(const Vector3i blockPosition) {
    this->blockIntersections.push_back(blockPosition);
}

const std::vector<Vector3i> RayCaster::BlockRay::getIntersections() const {
    return this->blockIntersections;
}

RayCaster::BlockRay RayCaster::castRay(const Vector3& vStart, const Vector3& vEnd, const int divisions) {
    BlockRay resultRay;

    const Vector3 step = (vEnd - vStart) / divisions;

    Vector3i lastIntersection = Chunk::worldToBlockPosition(vStart);
    for (int i = 0; i < divisions; i++) {
        const Vector3i intersection = Chunk::worldToBlockPosition(vStart + (step * i));

        if (intersection != lastIntersection) {
            lastIntersection = intersection;
            resultRay.addIntersection(intersection);
        }
    }

    return resultRay;
}

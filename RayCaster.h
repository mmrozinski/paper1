#ifndef RAYCASTER_H
#define RAYCASTER_H
#include <vector>

#include "Vector3i.h"


class RayCaster {
public:
    class BlockRay {
    private:
        std::vector<Vector3i> blockIntersections;
    public:
        const std::vector<Vector3i> getIntersections() const;
        void addIntersection(Vector3i blockPosition);
    };

    static BlockRay castRay(const Vector3& vStart, const Vector3& vEnd, int divisions);
};



#endif //RAYCASTER_H

#ifndef PLANE_H
#define PLANE_H
#include "Vector3.h"
#include <cmath>


class Plane {
public:
    float a;
    float b;
    float c;
    float d;

    Plane() {
        this->a = 0;
        this->b = 0;
        this->c = 0;
        this->d = 0;
    }

    Plane(float a, float b, float c, float d) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }

    Vector3 getNormal() {
        return Vector3(a, b, c);
    }

    float getPointDistance(Vector3 point) {
        return (a * point.x + b * point.y + c * point.z + d) / sqrtf(a * a + b * b + c * c);
    }
};



#endif //PLANE_H

#ifndef Vector3I_H
#define Vector3I_H
#include "Vector3.h"

struct  Vector3;

struct Vector3i {
public:
    int x, y, z;

    Vector3i();

    Vector3i(const Vector3& vec);

    Vector3i(int, int, int);

    Vector3i operator+(const Vector3i &vec) const;

    Vector3i operator+(int val) const;

    Vector3i operator-() const;

    Vector3i operator-(const Vector3i &vec) const;

    explicit operator Vector3() const;

    bool operator==(const Vector3i&) const;

    float length() const;

    static Vector3i unitX();

    static Vector3i unitY();

    static Vector3i unitZ();

    static Vector3i normalize(Vector3i vector);

    static Vector3i cross(const Vector3i &left, const Vector3i &right);

    static float dot(const Vector3i &left, const Vector3i &right);
};



#endif //Vector3I_H

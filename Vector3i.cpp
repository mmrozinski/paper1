#include "Vector3i.h"
#include <cmath>

Vector3i::Vector3i() {
    x = 0;
    y = 0;
    z = 0;
}

Vector3i::Vector3i(const int x, const int y, const int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3i Vector3i::operator+(const Vector3i &vec) const { return Vector3i(x + vec.x, y + vec.y, z + vec.z); }

Vector3i Vector3i::operator+(const int val) const { return Vector3i(x + val, y + val, z + val); }

Vector3i Vector3i::operator-() const {
    return Vector3i(-x, -y, -z);
}

Vector3i Vector3i::operator-(const Vector3i &vec) const {
    return Vector3i(x - vec.x, y - vec.y, z - vec.z);
}

Vector3i::operator Vector3() const {
    return Vector3(x, y, z);
}


float Vector3i::length() const {
    return sqrtf((x * x) + (y * y) + (z * z));
}

Vector3i Vector3i::unitX() { return Vector3i(1, 0, 0); }

Vector3i Vector3i::unitY() { return Vector3i(0, 1, 0); }

Vector3i Vector3i::unitZ() { return Vector3i(0, 0, 1); }

Vector3i Vector3i::normalize(Vector3i vector) {
    float scale = 1.0f / vector.length();

    vector.x *= scale;
    vector.y *= scale;
    vector.z *= scale;

    return vector;
}

Vector3i Vector3i::cross(const Vector3i &left, const Vector3i &right) {
    return Vector3i(
            (left.y * right.z) - (left.z * right.y),
            (left.z * right.x) - (left.x * right.z),
            (left.x * right.y) - (left.y * right.x));
}

float Vector3i::dot(const Vector3i &left, const Vector3i &right) {
    return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}


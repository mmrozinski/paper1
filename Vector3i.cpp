#include "Vector3i.h"
#include <cmath>

Vector3i::Vector3i() {
    x = 0;
    y = 0;
    z = 0;
}

Vector3i::Vector3i(const Vector3& vec) {
    x = floorf(vec.x);
    y = floorf(vec.y);
    z = floorf(vec.z);
}

Vector3i::Vector3i(const int x, const int y, const int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3i Vector3i::operator+(const Vector3i &vec) const { return {x + vec.x, y + vec.y, z + vec.z}; }

Vector3i Vector3i::operator+(const int val) const { return {x + val, y + val, z + val}; }

Vector3i Vector3i::operator-() const {
    return {-x, -y, -z};
}

Vector3i Vector3i::operator-(const Vector3i &vec) const {
    return {x - vec.x, y - vec.y, z - vec.z};
}

Vector3i::operator Vector3() const {
    return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};
}

bool Vector3i::operator==(const Vector3i& vec) const {
    return this->x == vec.x && this->y == vec.y && this->z == vec.z;
}


float Vector3i::length() const {
    return sqrtf((x * x) + (y * y) + (z * z));
}

Vector3i Vector3i::unitX() { return {1, 0, 0}; }

Vector3i Vector3i::unitY() { return {0, 1, 0}; }

Vector3i Vector3i::unitZ() { return {0, 0, 1}; }

Vector3i Vector3i::normalize(Vector3i vector) {
    float scale = 1.0f / vector.length();

    vector.x *= scale;
    vector.y *= scale;
    vector.z *= scale;

    return vector;
}

Vector3i Vector3i::cross(const Vector3i &left, const Vector3i &right) {
    return {(left.y * right.z) - (left.z * right.y),
            (left.z * right.x) - (left.x * right.z),
            (left.x * right.y) - (left.y * right.x)};
}

float Vector3i::dot(const Vector3i &left, const Vector3i &right) {
    return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}


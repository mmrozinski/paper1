#include "Vector3.h"
#include <cmath>

Vector3::Vector3() {
    x = 0;
    y = 0;
    z = 0;
}

Vector3::Vector3(const Vector3& vector3) {
    this->x = vector3.x;
    this->y = vector3.y;
    this->z = vector3.z;
}

Vector3::Vector3(const float x, const float y, const float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Vector3::operator+(const Vector3 &vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }

Vector3 Vector3::operator+(const float val) const { return Vector3(x + val, y + val, z + val); }

Vector3 Vector3::operator*(const float val) const {
    return Vector3(x * val, y * val, z * val);
}

Vector3 Vector3::operator/(const float val) const {
    return Vector3(x / val, y / val, z / val);
}

Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator-(const Vector3 &vec) const {
    return Vector3(x - vec.x, y - vec.y, z - vec.z);
}


float Vector3::length() const {
    return sqrtf((x * x) + (y * y) + (z * z));
}

Vector3 Vector3::unitX() { return Vector3(1, 0, 0); }

Vector3 Vector3::unitY() { return Vector3(0, 1, 0); }

Vector3 Vector3::unitZ() { return Vector3(0, 0, 1); }

Vector3 Vector3::normalize(Vector3 vector) {
    float scale = 1.0f / vector.length();

    vector.x *= scale;
    vector.y *= scale;
    vector.z *= scale;

    return vector;
}

Vector3 Vector3::cross(const Vector3 &left, const Vector3 &right) {
    return Vector3(
            (left.y * right.z) - (left.z * right.y),
            (left.z * right.x) - (left.x * right.z),
            (left.x * right.y) - (left.y * right.x));
}

float Vector3::dot(const Vector3 &left, const Vector3 &right) {
    return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}


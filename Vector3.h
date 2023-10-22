//
// Created by Makel on 10/22/2023.
//

#ifndef PAPER1_VECTOR3_H
#define PAPER1_VECTOR3_H

struct Vector3 {
public:
    float x, y, z;

    inline Vector3();

    inline Vector3(float, float, float);

    inline Vector3 operator+(const Vector3 &vec) const;

    inline Vector3 operator+(float val) const;

    inline Vector3 operator-() const;

    inline Vector3 operator-(const Vector3 &vec) const;

    inline float length() const;

    static Vector3 unitX();

    static Vector3 unitY();

    static Vector3 unitZ();

    static Vector3 normalize(Vector3 vector);

    static Vector3 cross(const Vector3 &left, const Vector3 &right);

    static float dot(const Vector3 &left, const Vector3 &right);
};

#endif //PAPER1_VECTOR3_H

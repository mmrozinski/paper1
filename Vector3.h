#ifndef PAPER1_VECTOR3_H
#define PAPER1_VECTOR3_H

struct Vector3 {
public:
    float x, y, z;

    Vector3();

    Vector3(const Vector3& vector3);

    Vector3(float, float, float);

    Vector3 operator+(const Vector3 &vec) const;

    Vector3 operator+(float val) const;

    Vector3 operator*(float val) const;

    Vector3 operator/(float val) const;

    Vector3 operator-() const;

    Vector3 operator-(const Vector3 &vec) const;

    friend Vector3 operator*(float lval, Vector3 rval);

    float length() const;

    static Vector3 unitX();

    static Vector3 unitY();

    static Vector3 unitZ();

    static Vector3 normalize(Vector3 vector);

    static Vector3 cross(const Vector3 &left, const Vector3 &right);

    static float dot(const Vector3 &left, const Vector3 &right);
};

#endif //PAPER1_VECTOR3_H

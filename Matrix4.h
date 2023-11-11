#ifndef PAPER1_MATRIX4_H
#define PAPER1_MATRIX4_H

#include "Vector3.h"
#include <cmath>

struct Matrix4 {
private:
    float _values[4][4];

public:
    Matrix4();
    Matrix4(float values[4][4]);
    Matrix4(const Matrix4 &matrix);

    float* operator[](std::size_t idx) { return _values[idx]; };
    const float* operator[](std::size_t idx) const { return _values[idx]; };

    static Matrix4 createIdentity();
    static Matrix4 lookAt(Vector3 eye, Vector3 target, Vector3 up);
    static Matrix4 createPerspectiveFieldOfView(float fov, float aspectRatio, float near, float far);
    static Matrix4 createTranslation(Vector3 vector);
};


#endif //PAPER1_MATRIX4_H

//
// Created by Makel on 10/22/2023.
//

#ifndef PAPER1_MATRIX4_H
#define PAPER1_MATRIX4_H

#include "Vector3.h"
#include <cmath>

struct Matrix4 {
private:
    float _values[4][4];

public:
    Matrix4();
    Matrix4(const Matrix4 &matrix);

    static Matrix4 lookAt(Vector3 eye, Vector3 target, Vector3 up);
    static Matrix4 createPerspectiveFieldOfView(float fov, float aspectRatio, float near, float far);
};


#endif //PAPER1_MATRIX4_H

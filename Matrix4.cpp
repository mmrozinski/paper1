//
// Created by Makel on 10/22/2023.
//

#include "Matrix4.h"

Matrix4::Matrix4(const Matrix4 &matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _values[i][j] = matrix._values[i][j];
        }
    }
}

Matrix4 Matrix4::lookAt(Vector3 eye, Vector3 target, Vector3 up) {
    Vector3 zVec = Vector3::normalize(eye - target);
    Vector3 xVec = Vector3::normalize(Vector3::cross(up, zVec));
    Vector3 yVec = Vector3::normalize(Vector3::cross(zVec, xVec));

    Matrix4 result;

    result._values[0][0] = xVec.x;
    result._values[1][0] = xVec.y;
    result._values[2][0] = xVec.z;
    result._values[3][0] = -Vector3::dot(xVec, eye);

    result._values[0][1] = yVec.x;
    result._values[1][1] = yVec.y;
    result._values[2][1] = yVec.z;
    result._values[3][1] = -Vector3::dot(yVec,eye);

    result._values[0][2] = zVec.x;
    result._values[1][2] = zVec.y;
    result._values[2][2] = zVec.z;
    result._values[3][2] = -Vector3::dot(zVec,eye);

    result._values[0][3] = 0.0f;
    result._values[1][3] = 0.0f;
    result._values[2][3] = 0.0f;
    result._values[3][3] = 1.0f;

    return result;
}

Matrix4 Matrix4::createPerspectiveFieldOfView(float fov, float aspectRatio, float near, float far) {
    Matrix4 result;

    float maxY = near * tanf(0.5f * fov);
    float minY = -maxY;
    float minX = minY * aspectRatio;
    float maxX = maxY * aspectRatio;

    float x = 2.0f * near / (maxX - minX);
    float y = 2.0f * near / (maxY - minY);
    float a = (maxX + minX) / (maxX - minX);
    float b = (maxY + minY) / (maxY - minY);
    float c = -(far + near) / (far - near);
    float d = -(2.0f * far * near) / (far - near);

    result._values[0][0] = x;
    result._values[1][0] = 0;
    result._values[2][0] = 0;
    result._values[3][0] = 0;

    result._values[0][1] = 0;
    result._values[1][1] = y;
    result._values[2][1] = 0;
    result._values[3][1] = 0;

    result._values[0][2] = a;
    result._values[1][2] = b;
    result._values[2][2] = c;
    result._values[3][2] = -1;

    result._values[0][3] = 0;
    result._values[1][3] = 0;
    result._values[2][3] = d;
    result._values[3][3] = 0;

    return result;
}

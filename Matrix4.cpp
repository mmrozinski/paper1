#include "Matrix4.h"

Matrix4::Matrix4() {
    for (auto & _value : _values) {
        for (int j = 0; j < 4; j++) {
            _value[j] = 0;
        }
    }
}

Matrix4::Matrix4(float (*values)[4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _values[i][j] = values[i][j];
        }
    }
}

Matrix4::Matrix4(const Matrix4 &matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _values[i][j] = matrix._values[i][j];
        }
    }
}

Matrix4 Matrix4::operator*(Matrix4 right) {
    float leftM11 = _values[0][0];
    float leftM12 = _values[0][1];
    float leftM13 = _values[0][2];
    float leftM14 = _values[0][3];
    float leftM21 = _values[1][0];
    float leftM22 = _values[1][1];
    float leftM23 = _values[1][2];
    float leftM24 = _values[1][3];
    float leftM31 = _values[2][0];
    float leftM32 = _values[2][1];
    float leftM33 = _values[2][2];
    float leftM34 = _values[2][3];
    float leftM41 = _values[3][0];
    float leftM42 = _values[3][1];
    float leftM43 = _values[3][2];
    float leftM44 = _values[3][3];
    float rightM11 = right[0][0];
    float rightM12 = right[0][1];
    float rightM13 = right[0][2];
    float rightM14 = right[0][3];
    float rightM21 = right[1][0];
    float rightM22 = right[1][1];
    float rightM23 = right[1][2];
    float rightM24 = right[1][3];
    float rightM31 = right[2][0];
    float rightM32 = right[2][1];
    float rightM33 = right[2][2];
    float rightM34 = right[2][3];
    float rightM41 = right[3][0];
    float rightM42 = right[3][1];
    float rightM43 = right[3][2];
    float rightM44 = right[3][3];

    Matrix4 result;

    result[0][0] = (leftM11 * rightM11) + (leftM12 * rightM21) + (leftM13 * rightM31) + (leftM14 * rightM41);
    result[0][1] = (leftM11 * rightM12) + (leftM12 * rightM22) + (leftM13 * rightM32) + (leftM14 * rightM42);
    result[0][2] = (leftM11 * rightM13) + (leftM12 * rightM23) + (leftM13 * rightM33) + (leftM14 * rightM43);
    result[0][3] = (leftM11 * rightM14) + (leftM12 * rightM24) + (leftM13 * rightM34) + (leftM14 * rightM44);
    result[1][0] = (leftM21 * rightM11) + (leftM22 * rightM21) + (leftM23 * rightM31) + (leftM24 * rightM41);
    result[1][1] = (leftM21 * rightM12) + (leftM22 * rightM22) + (leftM23 * rightM32) + (leftM24 * rightM42);
    result[1][2] = (leftM21 * rightM13) + (leftM22 * rightM23) + (leftM23 * rightM33) + (leftM24 * rightM43);
    result[1][3] = (leftM21 * rightM14) + (leftM22 * rightM24) + (leftM23 * rightM34) + (leftM24 * rightM44);
    result[2][0] = (leftM31 * rightM11) + (leftM32 * rightM21) + (leftM33 * rightM31) + (leftM34 * rightM41);
    result[2][1] = (leftM31 * rightM12) + (leftM32 * rightM22) + (leftM33 * rightM32) + (leftM34 * rightM42);
    result[2][2] = (leftM31 * rightM13) + (leftM32 * rightM23) + (leftM33 * rightM33) + (leftM34 * rightM43);
    result[2][3] = (leftM31 * rightM14) + (leftM32 * rightM24) + (leftM33 * rightM34) + (leftM34 * rightM44);
    result[3][0] = (leftM41 * rightM11) + (leftM42 * rightM21) + (leftM43 * rightM31) + (leftM44 * rightM41);
    result[3][1] = (leftM41 * rightM12) + (leftM42 * rightM22) + (leftM43 * rightM32) + (leftM44 * rightM42);
    result[3][2] = (leftM41 * rightM13) + (leftM42 * rightM23) + (leftM43 * rightM33) + (leftM44 * rightM43);
    result[3][3] = (leftM41 * rightM14) + (leftM42 * rightM24) + (leftM43 * rightM34) + (leftM44 * rightM44);

    return result;
}

Matrix4 Matrix4::createIdentity() {
    float v[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
                     {0.0f, 1.0f, 0.0f, 0.0f},
                     {0.0f, 0.0f, 1.0f, 0.0f},
                     {0.0f, 0.0f, 0.0f, 1.0f}};

    return {v};
}

Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
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
    result._values[3][1] = -Vector3::dot(yVec, eye);

    result._values[0][2] = zVec.x;
    result._values[1][2] = zVec.y;
    result._values[2][2] = zVec.z;
    result._values[3][2] = -Vector3::dot(zVec, eye);

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
    result._values[2][0] = a;
    result._values[3][0] = 0;

    result._values[0][1] = 0;
    result._values[1][1] = y;
    result._values[2][1] = b;
    result._values[3][1] = 0;

    result._values[0][2] = 0;
    result._values[1][2] = 0;
    result._values[2][2] = c;
    result._values[3][2] = d;

    result._values[0][3] = 0;
    result._values[1][3] = 0;
    result._values[2][3] = -1;
    result._values[3][3] = 0;

    return result;
}

Matrix4 Matrix4::createTranslation(const Vector3& vector) {
    Matrix4 result = createIdentity();

    result._values[3][0] = vector.x;
    result._values[3][1] = vector.y;
    result._values[3][2] = vector.z;

    return result;
}

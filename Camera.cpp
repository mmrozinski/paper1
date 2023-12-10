#include "Camera.h"


Camera::Camera(const Vector3& position, float aspectRatio) {
    _position = position;
    _aspectRatio = aspectRatio;
}

void Camera::updateVectors() {
    _front.x = cosf(_pitch) * cosf(_yaw);
    _front.y = sinf(_pitch);
    _front.z = cosf(_pitch) * sinf(_yaw);

    _right = Vector3::cross(_front, _up);

    _front = Vector3::normalize(_front);
    _right = Vector3::normalize(_right);
    _up = Vector3::normalize(_up);
}

Matrix4 Camera::getViewMatrix() const {
    return Matrix4::lookAt(_position, _position + _front, _up);
}

Matrix4 Camera::getProjectionMatrix() const {
    return Matrix4::createPerspectiveFieldOfView(_fov, _aspectRatio, 0.01f, 20000.0f);
}

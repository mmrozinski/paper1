#ifndef PAPER1_CAMERA_H
#define PAPER1_CAMERA_H

#include "Vector3.h"
#include "Matrix4.h"
#include <cmath>

class Camera {
private:
    Vector3 _position;

    Vector3 _front = -Vector3::unitZ();
    Vector3 _up = Vector3::unitY();
    Vector3 _right = Vector3::unitX();

    // Rotation around x (rad)
    float _pitch;

    // Rotation around y (rad)
    float _yaw = -M_PI / 2.0f;

    float _fov = M_PI / 2.0f;
    float _aspectRatio;

public:
    Camera(Vector3 position, float aspectRatio);

    Vector3 getPosition() { return _position; }

    void setPosition(Vector3 position) { _position = position; }

    void setAspectRatio(float aspectRatio) { _aspectRatio = aspectRatio; }

    void updateVectors();

    Matrix4 getViewMatrix() const;

    Matrix4 getProjectionMatrix() const;

    const Vector3 &getFront() const {
        return _front;
    }

    void setFront(const Vector3 &front) {
        _front = front;
    }

    const Vector3 &getUp() const {
        return _up;
    }

    void setUp(const Vector3 &up) {
        _up = up;
    }

    const Vector3 &getRight() const {
        return _right;
    }

    void setRight(const Vector3 &right) {
        _right = right;
    }

    // Get pitch (deg)
    float getPitch() const {
        return _pitch * 180.0f / M_PI;
    }

    // Set pitch (deg)
    void setPitch(float pitch) {
        _pitch = pitch;
    }

    // Get yaw (deg)
    float getYaw() const {
        return _yaw * 180.0f / M_PI;
    }

    // Set yaw (deg)
    void setYaw(float yaw) {
        _yaw = yaw;
    }

    // Get fov (deg)
    float getFov() const {
        return _fov * 180.0f / M_PI;
    }

    // Set fov (deg)
    void setFov(float fov) {
        _fov = fov;
    }
};

#endif //PAPER1_CAMERA_H

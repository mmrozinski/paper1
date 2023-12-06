#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "Matrix4.h"
#include "MeshRenderer.h"
#include "Plane.h"


class Frustum {
private:
    static Vector3 intersectionPoint(Plane a, Plane b, Plane c);
    Plane planes[6];
public:
    enum Sides { FRUSTUM_TOP, FRUSTUM_BOTTOM, FRUSTUM_LEFT, FRUSTUM_RIGHT, FRUSTUM_NEAR, FRUSTUM_FAR };
    enum Intersections { FRUSTUM_OUTSIDE, FRUSTUM_INTERSECT, FRUSTUM_INSIDE };

    Frustum();
    explicit Frustum(Matrix4 matrix);
    Intersections pointInFrustum(const Vector3& point);
    Intersections sphereInFrustum(const Vector3& point, float radius);
    Intersections cubeInFrustum(const Vector3& center, float x, float y, float z);

    void preRender(MeshRenderer* renderer);
    void render(MeshRenderer* renderer);
};


#endif //FRUSTUM_H

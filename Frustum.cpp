#include "Frustum.h"

Vector3 Frustum::intersectionPoint(Plane a, Plane b, Plane c) {
    Vector3 v1, v2, v3;
    float f = -Vector3::dot(a.getNormal(), Vector3::cross(b.getNormal(), c.getNormal()));

    v1 = (a.d * (Vector3::cross(b.getNormal(), c.getNormal())));
    v2 = (b.d * (Vector3::cross(c.getNormal(), a.getNormal())));
    v3 = (c.d * (Vector3::cross(a.getNormal(), b.getNormal())));

    Vector3 vec = Vector3(v1.x + v2.x + v3.x, v1.y + v2.y + v3.y, v1.z + v2.z + v3.z);
    return vec / f;
}

Frustum::Frustum() {
    Frustum(Matrix4());
}

Frustum::Frustum(Matrix4 matrix) {
    planes[0] = Plane(matrix[0][3] - matrix[0][1], matrix[1][3] - matrix[1][1], matrix[2][3] - matrix[2][1], matrix[3][3] - matrix[3][1]);
    planes[1] = Plane(matrix[0][3] + matrix[0][1], matrix[1][3] + matrix[1][1], matrix[2][3] + matrix[2][1], matrix[3][3] + matrix[3][1]);
    planes[2] = Plane(matrix[0][3] + matrix[0][0], matrix[1][3] + matrix[1][0], matrix[2][3] + matrix[2][0], matrix[3][3] + matrix[3][0]);
    planes[3] = Plane(matrix[0][3] - matrix[0][0], matrix[1][3] - matrix[1][0], matrix[2][3] - matrix[2][0], matrix[3][3] - matrix[3][0]);
    planes[4] = Plane(matrix[0][3] + matrix[0][2], matrix[1][3] + matrix[1][2], matrix[2][3] + matrix[2][2], matrix[3][3] + matrix[3][2]);
    planes[5] = Plane(matrix[0][3] - matrix[0][2], matrix[1][3] - matrix[1][2], matrix[2][3] - matrix[2][2], matrix[3][3] - matrix[3][2]);
}

Frustum::Intersections Frustum::pointInFrustum(const Vector3& point) {
    for (int i = 0; i < 6; i++) {
        if (planes[i].getPointDistance(point) < 0) {
            return FRUSTUM_OUTSIDE;
        }
    }
    return FRUSTUM_INSIDE;
}

Frustum::Intersections Frustum::sphereInFrustum(const Vector3& point, float radius) {
    Intersections result = FRUSTUM_INSIDE;
    for (int i = 0; i < 6; i++) {
        float distance = planes[i].getPointDistance(point);
        if (distance < -radius) {
            return FRUSTUM_OUTSIDE;
        } else if (distance < radius) {
            result = FRUSTUM_INTERSECT;
        }
    }
    return result;
}

Frustum::Intersections Frustum::cubeInFrustum(const Vector3& center, float x, float y, float z) {
    Intersections result = FRUSTUM_INSIDE;
    for (int i = 0; i < 6; i++) {
        int outside = 0;
        int inside = 0;

        if (planes[i].getPointDistance(center + Vector3(-x, -y, -z)) < 0)
            outside++;
        else
            inside++;

        if (planes[i].getPointDistance(center + Vector3(x, -y, -z)) < 0)
            outside++;
        else
            inside++;

        if (planes[i].getPointDistance(center + Vector3(-x, -y, z)) < 0)
            outside++;
        else
            inside++;

        if (planes[i].getPointDistance(center + Vector3(x, -y, z)) < 0)
            outside++;
        else
            inside++;

        if (planes[i].getPointDistance(center + Vector3(-x, y, -z)) < 0)
            outside++;
        else
            inside++;

        if (planes[i].getPointDistance(center + Vector3(x, y, -z)) < 0)
            outside++;
        else
            inside++;

        if (planes[i].getPointDistance(center + Vector3(-x, y, z)) < 0)
            outside++;
        else
            inside++;

        if (planes[i].getPointDistance(center + Vector3(x, y, z)) < 0)
            outside++;
        else
            inside++;

        if (inside == 0)
            return FRUSTUM_OUTSIDE;
        else if (outside != 0)
            result = FRUSTUM_INTERSECT;
    }
    return result;
}

void Frustum::preRender(MeshRenderer* renderer) {
    renderer->clear();

    Vector3 p1 = intersectionPoint(planes[0], planes[2], planes[4]);
    Vector3 p2 = intersectionPoint(planes[0], planes[3], planes[4]);
    Vector3 p3 = intersectionPoint(planes[1], planes[3], planes[4]);
    Vector3 p4 = intersectionPoint(planes[1], planes[2], planes[4]);
    Vector3 p5 = intersectionPoint(planes[0], planes[2], planes[5]);
    Vector3 p6 = intersectionPoint(planes[0], planes[3], planes[5]);
    Vector3 p7 = intersectionPoint(planes[1], planes[2], planes[5]);
    Vector3 p8 = intersectionPoint(planes[1], planes[3], planes[5]);

    // auto v1 = static_cast<unsigned int>(renderer->addVertexToMesh(p1, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0.0f, 0.0f, 0.0f)));
    // auto v2 = static_cast<unsigned int>(renderer->addVertexToMesh(p2, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1.0f, 0.0f, 0.0f)));
    // auto v3 = static_cast<unsigned int>(renderer->addVertexToMesh(p3, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1.0f, 1.0f, 0.0f)));
    // auto v4 = static_cast<unsigned int>(renderer->addVertexToMesh(p4, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0.0f, 1.0f, 0.0f)));
    // auto v5 = static_cast<unsigned int>(renderer->addVertexToMesh(p5, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1.0f, 1.0f, 0.0f)));
    // auto v6 = static_cast<unsigned int>(renderer->addVertexToMesh(p6, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0.0f, 1.0f, 0.0f)));
    // auto v7 = static_cast<unsigned int>(renderer->addVertexToMesh(p7, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0.0f, 0.0f, 0.0f)));
    // auto v8 = static_cast<unsigned int>(renderer->addVertexToMesh(p8, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1.0f, 0.0f, 0.0f)));

    Vector3 n;

    unsigned int v1;
    unsigned int v2;
    unsigned int v3;
    unsigned int v4;
    unsigned int v5;
    unsigned int v6;
    unsigned int v7;
    unsigned int v8;

    Vector3 color(1.0f, 0.0f, 0.0f);

    Vector3 topLeft(0.0f, 1.0f, 0.0f);
    Vector3 topRight(1.0f, 1.0f, 0.0f);
    Vector3 bottomLeft(0.0f, 0.0f, 0.0f);
    Vector3 bottomRight(1.0f, 0.0f, 0.0f);

    // Front
    n = planes[4].getNormal();
    v1 = static_cast<unsigned int>(renderer->addVertexToMesh(p1, n, color, bottomLeft));
    v2 = static_cast<unsigned int>(renderer->addVertexToMesh(p2, n, color, bottomRight));
    v3 = static_cast<unsigned int>(renderer->addVertexToMesh(p3, n, color, topRight));
    v4 = static_cast<unsigned int>(renderer->addVertexToMesh(p4, n, color, topLeft));
    renderer->addTriangleToMesh(v1, v2, v3);
    renderer->addTriangleToMesh(v1, v3, v4);

    // Back
    n = planes[5].getNormal();
    v5 = static_cast<unsigned int>(renderer->addVertexToMesh(p5, n, color, bottomLeft));
    v6 = static_cast<unsigned int>(renderer->addVertexToMesh(p6, n, color, bottomRight));
    v7 = static_cast<unsigned int>(renderer->addVertexToMesh(p7, n, color, topRight));
    v8 = static_cast<unsigned int>(renderer->addVertexToMesh(p8, n, color, topLeft));
    renderer->addTriangleToMesh(v5, v6, v7);
    renderer->addTriangleToMesh(v5, v7, v8);

    // Right
    n = planes[3].getNormal();
    v2 = static_cast<unsigned int>(renderer->addVertexToMesh(p2, n, color, bottomLeft));
    v5 = static_cast<unsigned int>(renderer->addVertexToMesh(p5, n, color, bottomRight));
    v8 = static_cast<unsigned int>(renderer->addVertexToMesh(p8, n, color, topRight));
    v3 = static_cast<unsigned int>(renderer->addVertexToMesh(p3, n, color, topLeft));
    renderer->addTriangleToMesh(v2, v5, v8);
    renderer->addTriangleToMesh(v2, v8, v3);

    // Left
    n = planes[2].getNormal();
    v6 = static_cast<unsigned int>(renderer->addVertexToMesh(p6, n, color, bottomLeft));
    v1 = static_cast<unsigned int>(renderer->addVertexToMesh(p1, n, color, bottomRight));
    v4 = static_cast<unsigned int>(renderer->addVertexToMesh(p4, n, color, topRight));
    v7 = static_cast<unsigned int>(renderer->addVertexToMesh(p7, n, color, topLeft));
    renderer->addTriangleToMesh(v6, v1, v4);
    renderer->addTriangleToMesh(v6, v4, v7);

    // Top
    n = planes[1].getNormal();
    v4 = static_cast<unsigned int>(renderer->addVertexToMesh(p4, n, color, bottomLeft));
    v3 = static_cast<unsigned int>(renderer->addVertexToMesh(p3, n, color, bottomRight));
    v8 = static_cast<unsigned int>(renderer->addVertexToMesh(p8, n, color, topRight));
    v7 = static_cast<unsigned int>(renderer->addVertexToMesh(p7, n, color, topLeft));
    renderer->addTriangleToMesh(v4, v3, v8);
    renderer->addTriangleToMesh(v4, v8, v7);

    // Bottom
    n = planes[0].getNormal();
    v6 = static_cast<unsigned int>(renderer->addVertexToMesh(p6, n, color, bottomLeft));
    v5 = static_cast<unsigned int>(renderer->addVertexToMesh(p5, n, color, bottomRight));
    v2 = static_cast<unsigned int>(renderer->addVertexToMesh(p2, n, color, topRight));
    v1 = static_cast<unsigned int>(renderer->addVertexToMesh(p1, n, color, topLeft));
    renderer->addTriangleToMesh(v6, v5, v2);
    renderer->addTriangleToMesh(v6, v2, v1);
}

void Frustum::render(MeshRenderer* renderer) {
    renderer->render();
}

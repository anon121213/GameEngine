#include "RenderObjectFactory.hpp"
#include "render/components/RenderMeshComponent.hpp"
#include "components/Transform.hpp"
#include <glm/glm.hpp>

RenderMeshComponent RenderObjectFactory::CreateTriangle() {
    RenderMeshComponent mesh;

    mesh.vertices = {
        {{0.0f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };

    mesh.indices = { 0, 1, 2 };
    mesh.initialized = false;

    return mesh;
}

RenderMeshComponent RenderObjectFactory::CreateCube() {
    RenderMeshComponent mesh;
    mesh.vertices = {
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}}, {{0.5f, -0.5f, -0.5f}, {0, 1, 0}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 1}},  {{-0.5f, 0.5f, -0.5f}, {1, 1, 0}},
        {{-0.5f, -0.5f, 0.5f}, {1, 0, 1}}, {{0.5f, -0.5f, 0.5f}, {0, 1, 1}},
        {{0.5f, 0.5f, 0.5f}, {1, 1, 1}},   {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}}
    };

    mesh.indices = {
        0,1,2, 2,3,0,  // back
        4,5,6, 6,7,4,  // front
        4,5,1, 1,0,4,  // bottom
        3,2,6, 6,7,3,  // top
        4,0,3, 3,7,4,  // left
        1,5,6, 6,2,1   // right
    };

    mesh.initialized = false;
    return mesh;
}

RenderMeshComponent RenderObjectFactory::CreatePlane(float size) {
    RenderMeshComponent mesh;
    float hs = size / 2.0f;

    mesh.vertices = {
        {{-hs, 0.0f, -hs}, {1, 0, 0}},
        {{hs, 0.0f, -hs}, {0, 1, 0}},
        {{hs, 0.0f, hs}, {0, 0, 1}},
        {{-hs, 0.0f, hs}, {1, 1, 0}}
    };

    mesh.indices = {
        0, 1, 2,
        2, 3, 0
    };

    mesh.initialized = false;
    return mesh;
}

Transform RenderObjectFactory::CreateTransform(const glm::vec3& position,
                                               const glm::vec3& rotation,
                                               const glm::vec3& scale) {
    Transform transform;
    transform.position = position;
    transform.rotation = rotation;
    transform.scale = scale;
    return transform;
}

CameraComponent RenderObjectFactory::CreateCamera(float fov, float nearZ, float farZ, bool primary) {
    CameraComponent cam;
    cam.fov = fov;
    cam.nearZ = nearZ;
    cam.farZ = farZ;
    cam.primary = primary;
    return cam;
}

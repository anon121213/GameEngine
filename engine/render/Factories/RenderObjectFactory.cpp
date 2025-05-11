#include "RenderObjectFactory.hpp"
#include "render/components/RenderMeshComponent.hpp"
#include "components/Transform.hpp"
#include <glm/glm.hpp>

#include "core/Log.hpp"
#include "ecs/Entity.hpp"
#include "ecs/World.hpp"
#include "render/dx12/services/ModelLoaderService.hpp"
#include "services/ServiceLocator.hpp"

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
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 1, 1}}, {{0.5f, -0.5f, -0.5f}, {0, 1, 0}, {1, 1, 1}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 1}, {1, 1, 1}},  {{-0.5f, 0.5f, -0.5f}, {1, 1, 0}, {1, 1, 1}},
        {{-0.5f, -0.5f, 0.5f}, {1, 0, 1}, {1, 1, 1}}, {{0.5f, -0.5f, 0.5f}, {0, 1, 1}, {1, 1, 1}},
        {{0.5f, 0.5f, 0.5f}, {1, 1, 1}, {1, 1, 1}},   {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1, 1, 1}}
    };

    mesh.indices = {
        2,1,0, 0,3,2, 
        4,5,6, 6,7,4,  
        1,5,4, 4,0,1, 
        6,2,3, 3,7,6,  
        3,0,4, 4,7,3, 
        1,2,6, 6,5,1   
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

RenderMeshComponent RenderObjectFactory::GetFBXMesh(const std::string &path) {
    auto modelLoaderService = GET_SERVICE(ModelLoaderService);
    auto modelMeshes = modelLoaderService->LoadModel(path);

    RenderMeshComponent combinedMesh;
    uint32_t indexOffset = 0;

    for (const auto& mesh : modelMeshes) {
        combinedMesh.vertices.insert(
            combinedMesh.vertices.end(),
            mesh.vertices.begin(),
            mesh.vertices.end()
        );

        for (uint32_t idx : mesh.indices) {
            combinedMesh.indices.push_back(idx + indexOffset);
        }

        indexOffset += static_cast<uint32_t>(mesh.vertices.size());
    }
    
    return combinedMesh;
}

#include "RenderObjectFactory.hpp"
#include "render/components/RenderMeshComponent.hpp"
#include "components/Transform.hpp"
#include "render/components/MeshAsset.hpp"
#include "assets/MeshManager.hpp"
#include "core/Log.hpp"
#include <glm/glm.hpp>
#include "render/dx12/services/ModelLoaderService.hpp"
#include "services/ServiceLocator.hpp"

RenderMeshComponent RenderObjectFactory::CreateTriangle()
{
    MeshAsset asset;
    asset.path = "Primitive:Triangle";

    asset.vertices = {
        {{0.0f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };

    asset.indices = { 0, 1, 2 };

    ServiceLocator::Get<MeshManager>()->RegisterPrimitive(asset);

    RenderMeshComponent mesh{};
    mesh.constantBufferIndex = GetNextConstantBufferIndex();

    SubMesh sub{};
    sub.meshPath = asset.path;
    sub.initialized = false;

    mesh.subMeshes.push_back(std::move(sub));
    return mesh;
}

RenderMeshComponent RenderObjectFactory::CreateCube()
{
    MeshAsset asset;
    asset.path = "Primitive:Cube";

    asset.vertices = {
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
        {{0.5f, -0.5f, -0.5f},  {0, 1, 0}},
        {{0.5f,  0.5f, -0.5f},  {0, 0, 1}},
        {{-0.5f,  0.5f, -0.5f}, {1, 1, 0}},
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 1}},
        {{0.5f, -0.5f,  0.5f},  {0, 1, 1}},
        {{0.5f,  0.5f,  0.5f},  {1, 1, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 0}}
    };

    asset.indices = {
        2,1,0, 0,3,2,
        4,5,6, 6,7,4,
        1,5,4, 4,0,1,
        6,2,3, 3,7,6,
        3,0,4, 4,7,3,
        1,2,6, 6,5,1
    };

    ServiceLocator::Get<MeshManager>()->RegisterPrimitive(asset);

    RenderMeshComponent mesh{};
    mesh.constantBufferIndex = GetNextConstantBufferIndex();

    SubMesh sub{};
    sub.meshPath = asset.path;
    sub.initialized = false;

    mesh.subMeshes.push_back(std::move(sub));
    return mesh;
}

RenderMeshComponent RenderObjectFactory::CreatePlane(const float size)
{
    MeshAsset asset;
    asset.path = "Primitive:Plane";

    float hs = size / 2.0f;

    asset.vertices = {
        {{-hs, 0.0f, -hs}, {1, 0, 0}},
        {{ hs, 0.0f, -hs}, {0, 1, 0}},
        {{ hs, 0.0f,  hs}, {0, 0, 1}},
        {{-hs, 0.0f,  hs}, {1, 1, 0}}
    };

    asset.indices = { 0, 1, 2, 2, 3, 0 };

    ServiceLocator::Get<MeshManager>()->RegisterPrimitive(asset);

    RenderMeshComponent mesh{};
    mesh.constantBufferIndex = GetNextConstantBufferIndex();

    SubMesh sub{};
    sub.meshPath = asset.path;
    sub.initialized = false;

    mesh.subMeshes.push_back(std::move(sub));
    return mesh;
}

Transform RenderObjectFactory::CreateTransform(const glm::vec3& position,
                                               const glm::vec3& rotation,
                                               const glm::vec3& scale)
{
    Transform transform;
    transform.position = position;
    transform.rotation = rotation;
    transform.scale = scale;
    return transform;
}

CameraComponent RenderObjectFactory::CreateCamera(const float fov, const float nearZ, const float farZ, const bool primary)
{
    CameraComponent cam;
    cam.fov = fov;
    cam.nearZ = nearZ;
    cam.farZ = farZ;
    cam.primary = primary;
    return cam;
}

RenderMeshComponent RenderObjectFactory::GetFBXMesh(const std::string& path)
{
    const auto modelLoader = ServiceLocator::Get<ModelLoaderService>();
    const std::vector<MeshAsset> meshes = modelLoader->LoadModel(path);

    RenderMeshComponent renderMesh{};
    renderMesh.constantBufferIndex = GetNextConstantBufferIndex();

    if (meshes.empty())
    {
        LOG_ERROR("[RenderObjectFactory] model '{}' contains NO meshes!", path);
        return renderMesh;
    }

    const auto meshManager = ServiceLocator::Get<MeshManager>();
    for (auto& asset : meshes)
    {
        meshManager->RegisterPrimitive(asset);

        SubMesh sub{};
        sub.meshPath = asset.path;    
        sub.materialPath = "";        
        sub.initialized = false;

        renderMesh.subMeshes.emplace_back(std::move(sub));
        LOG_INFO("[RenderObjectFactory] added submesh key='{}' to RenderMesh", sub.meshPath);
    }

    LOG_INFO("[RenderObjectFactory] FBX '{}' -> {} submeshes (CB index = {})",
             path, renderMesh.subMeshes.size(), renderMesh.constantBufferIndex);

    return renderMesh;
}

static uint32_t cbIndexCounter = 0;
uint32_t RenderObjectFactory::GetNextConstantBufferIndex()
{
    return cbIndexCounter++;
}

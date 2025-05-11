#include "ModelLoaderService.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <DirectXMath.h>
#include "render/Factories/RenderObjectFactory.hpp"

std::vector<RenderMeshComponent> ModelLoaderService::LoadModel(const std::string& path) {
    std::vector<RenderMeshComponent> meshes;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType |
        aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error("Assimp: " + std::string(importer.GetErrorString()));
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* ai_mesh = scene->mMeshes[i];
        RenderMeshComponent mesh;

        for (unsigned int v = 0; v < ai_mesh->mNumVertices; ++v) {
            Vertex vertex;
            vertex.position = {
                ai_mesh->mVertices[v].x,
                ai_mesh->mVertices[v].y,
                ai_mesh->mVertices[v].z
            };

            vertex.normal = ai_mesh->HasNormals()
                ? DirectX::XMFLOAT3{
                    ai_mesh->mNormals[v].x,
                    ai_mesh->mNormals[v].y,
                    ai_mesh->mNormals[v].z
                }
                : DirectX::XMFLOAT3{0, 0, 0};

            vertex.color = {1, 1, 1};

            mesh.vertices.push_back(vertex);
        }

        for (unsigned int f = 0; f < ai_mesh->mNumFaces; ++f) {
            const aiFace& face = ai_mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                mesh.indices.push_back(face.mIndices[j]);
            }
        }

        mesh.initialized = false;
        mesh.constantBufferIndex = RenderObjectFactory::GetNextConstantBufferIndex();
        meshes.push_back(std::move(mesh));
    }

    return meshes;
}

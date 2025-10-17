#define NOMINMAX
#include "ModelLoaderService.hpp"
#include "render/components/MeshKey.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/matrix_inverse.hpp>
#include "core/Log.hpp"
#include <algorithm> // для std::min/std::max


static void ComputeAABB(const std::vector<Vertex>& verts,
                        glm::vec3& minV, glm::vec3& maxV)
{
    if (verts.empty()) { minV = maxV = {0,0,0}; return; }
    minV = maxV = { verts[0].position.x, verts[0].position.y, verts[0].position.z };
    for (const auto& v : verts) {
        minV.x = std::min(minV.x, v.position.x);
        minV.y = std::min(minV.y, v.position.y);
        minV.z = std::min(minV.z, v.position.z);
        maxV.x = std::max(maxV.x, v.position.x);
        maxV.y = std::max(maxV.y, v.position.y);
        maxV.z = std::max(maxV.z, v.position.z);
    }
}

std::vector<MeshAsset> ModelLoaderService::LoadModel(const std::string& path) {
    std::vector<MeshAsset> loadedMeshes;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType |
        aiProcess_FlipUVs
    );

    if (!scene || !scene->mRootNode) {
        LOG_ERROR("[ModelLoader] Failed to load '{}': {}", path, importer.GetErrorString());
        return {};
    }

    ProcessNode(scene->mRootNode, scene, loadedMeshes, glm::mat4(1.0f), path);
    LOG_INFO("[ModelLoader] Loaded {} submeshes from '{}'", loadedMeshes.size(), path);
    return loadedMeshes;
}

void ModelLoaderService::ProcessNode(aiNode* node, const aiScene* scene,
                                     std::vector<MeshAsset>& meshes,
                                     const glm::mat4& parentTransform,
                                     const std::string& modelPath)
{
    glm::mat4 local = glm::transpose(glm::make_mat4(&node->mTransformation.a1));
    glm::mat4 global = parentTransform * local;

    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];

        MeshAsset asset;

        const char* rawName = ai_mesh->mName.C_Str();
        std::string nodeName = node->mName.C_Str();
        std::string meshName = (rawName && rawName[0] != '\0') 
            ? std::string(rawName)
            : ("mesh_" + std::to_string(i));

        asset.path = MakeSubmeshKey(modelPath, nodeName + "::" + meshName + "_" + std::to_string(i));


        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(global)));
        asset.vertices.reserve(ai_mesh->mNumVertices);
        asset.indices.reserve(ai_mesh->mNumFaces * 3);

        for (unsigned int v = 0; v < ai_mesh->mNumVertices; ++v) {
            Vertex vertex{};
            glm::vec4 pos(ai_mesh->mVertices[v].x, ai_mesh->mVertices[v].y, ai_mesh->mVertices[v].z, 1.0f);
            pos = global * pos;
            vertex.position = { pos.x, pos.y, pos.z };

            if (ai_mesh->HasNormals()) {
                glm::vec3 norm(ai_mesh->mNormals[v].x, ai_mesh->mNormals[v].y, ai_mesh->mNormals[v].z);
                norm = glm::normalize(normalMat * norm);
                vertex.normal = { norm.x, norm.y, norm.z };
            } else {
                vertex.normal = { 0, 1, 0 };
            }

            vertex.color = { 1, 1, 1 };
            asset.vertices.push_back(vertex);
        }

        for (unsigned int f = 0; f < ai_mesh->mNumFaces; ++f) {
            const aiFace& face = ai_mesh->mFaces[f];
            if (face.mNumIndices != 3) {
                LOG_WARN("[ModelLoader] non-triangle face ({} indices) in '{}'", face.mNumIndices, asset.path);
                continue;
            }
            asset.indices.push_back(face.mIndices[0]);
            asset.indices.push_back(face.mIndices[1]);
            asset.indices.push_back(face.mIndices[2]);
        }

        glm::vec3 minV, maxV;
        ComputeAABB(asset.vertices, minV, maxV);
        LOG_INFO("[ModelLoader] submesh='{}' verts={} idx={} AABB min=({:.3f},{:.3f},{:.3f}) max=({:.3f},{:.3f},{:.3f})",
                 asset.path, asset.vertices.size(), asset.indices.size(),
                 minV.x, minV.y, minV.z, maxV.x, maxV.y, maxV.z);

        meshes.push_back(std::move(asset));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
        ProcessNode(node->mChildren[i], scene, meshes, global, modelPath);
}

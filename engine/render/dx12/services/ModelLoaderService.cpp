#include "ModelLoaderService.hpp"

#include <functional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>

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

    std::function<void(aiNode*, const aiScene*)> processNode;
    processNode = [&](aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
            RenderMeshComponent mesh;

            mesh.vertices.reserve(ai_mesh->mNumVertices);
            for (unsigned int v = 0; v < ai_mesh->mNumVertices; ++v) {
                Vertex vertex;
                vertex.position = {
                    ai_mesh->mVertices[v].x,
                    ai_mesh->mVertices[v].y,
                    ai_mesh->mVertices[v].z
                };

                if (ai_mesh->HasNormals()) {
                    vertex.normal = {
                        ai_mesh->mNormals[v].x,
                        ai_mesh->mNormals[v].y,
                        ai_mesh->mNormals[v].z
                    };
                } else {
                    vertex.normal = { 0.0f, 0.0f, 0.0f };
                }

                // Задаём цвет по умолчанию, так как материалы не поддерживаются
                vertex.color = { 1.0f, 1.0f, 1.0f };

                mesh.vertices.push_back(vertex);
            }

            mesh.indices.reserve(ai_mesh->mNumFaces * 3);
            for (unsigned int f = 0; f < ai_mesh->mNumFaces; ++f) {
                const aiFace& face = ai_mesh->mFaces[f];
                for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                    mesh.indices.push_back(face.mIndices[j]);
                }
            }

            mesh.initialized = false;
            meshes.push_back(std::move(mesh));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    };

    processNode(scene->mRootNode, scene);
    return meshes;
}

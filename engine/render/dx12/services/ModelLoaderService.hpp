#pragma once
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include "render/components/MeshAsset.hpp"

struct aiNode;
struct aiScene;

class ModelLoaderService {
public:
  std::vector<MeshAsset> LoadModel(const std::string& path);

private:
  void ProcessNode(aiNode* node,
                   const aiScene* scene,
                   std::vector<MeshAsset>& meshes,
                   const glm::mat4& parentTransform,
                   const std::string& modelPath);
};

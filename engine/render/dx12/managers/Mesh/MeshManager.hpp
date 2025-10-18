#pragma once
#include <string>
#include <unordered_map>
#include "render/data/mesh/MeshAsset.hpp"

class MeshManager {
public:
  void RegisterPrimitive(const MeshAsset& asset);
  const MeshAsset& LoadMesh(const std::string& path);

private:
  std::unordered_map<std::string, MeshAsset> meshes;
  MeshAsset empty;
};

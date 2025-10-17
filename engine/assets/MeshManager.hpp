#pragma once
#include <string>
#include <unordered_map>
#include "render/components/MeshAsset.hpp"

class MeshManager {
public:
  void RegisterPrimitive(const MeshAsset& asset);
  const MeshAsset& LoadMesh(const std::string& path); // возвращаем const&; храним внутри

private:
  std::unordered_map<std::string, MeshAsset> meshes;
  MeshAsset empty; // безопасная заглушка
};

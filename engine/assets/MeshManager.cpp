#include "assets/MeshManager.hpp"
#include "core/Log.hpp"

void MeshManager::RegisterPrimitive(const MeshAsset& asset) {
  if (asset.path.empty()) {
    LOG_ERROR("[MeshManager] attempt to register mesh with EMPTY path key.");
    return;
  }
  auto it = meshes.find(asset.path);
  if (it != meshes.end()) {
    LOG_WARN("[MeshManager] mesh key '{}' already exists, overwriting.", asset.path);
    it->second = asset;
  } else {
    meshes.emplace(asset.path, asset);
  }
  LOG_INFO("[MeshManager] registered mesh key='{}' verts={} idx={}",
           asset.path, asset.vertices.size(), asset.indices.size());
}

const MeshAsset& MeshManager::LoadMesh(const std::string& path) {
  auto it = meshes.find(path);
  if (it == meshes.end()) {
    LOG_ERROR("[MeshManager] mesh key '{}' NOT FOUND. Did you build key as 'file::meshName'?", path);
    return empty;
  }
  return it->second;
}

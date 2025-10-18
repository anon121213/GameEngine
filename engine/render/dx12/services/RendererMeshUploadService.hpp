#pragma once
#include <d3d12.h>
#include "components/rendering/RenderMeshComponent.hpp"
#include "render/data/Mesh/MeshAsset.hpp"

class RendererMeshUploadService {
public:
  static void UploadIfNeeded(ID3D12Device* device, SubMesh& subMesh);

private:
  static void CreateVertexBuffer(ID3D12Device* device, SubMesh& subMesh, const MeshAsset& asset);
  static void CreateIndexBuffer(ID3D12Device* device, SubMesh& subMesh, const MeshAsset& asset);
};

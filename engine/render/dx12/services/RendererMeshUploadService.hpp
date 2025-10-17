#pragma once
#include <d3d12.h>
#include "render/components/RenderMeshComponent.hpp"
#include "render/components/MeshAsset.hpp"
#include "assets/MeshManager.hpp"

// Сервис, который выгружает SubMesh в видеопамять при первом использовании
class RendererMeshUploadService {
public:
  static void UploadIfNeeded(ID3D12Device* device, SubMesh& subMesh);

private:
  static void CreateVertexBuffer(ID3D12Device* device, SubMesh& subMesh, const MeshAsset& asset);
  static void CreateIndexBuffer(ID3D12Device* device, SubMesh& subMesh, const MeshAsset& asset);
};

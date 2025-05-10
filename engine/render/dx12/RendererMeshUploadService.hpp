#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "render/components/RenderMeshComponent.hpp"

class RendererMeshUploadService {
public:
  void UploadIfNeeded(ID3D12Device* device, RenderMeshComponent& mesh);

private:
  void CreateVertexBuffer(ID3D12Device* device, RenderMeshComponent& mesh);
  void CreateIndexBuffer(ID3D12Device* device, RenderMeshComponent& mesh);
};

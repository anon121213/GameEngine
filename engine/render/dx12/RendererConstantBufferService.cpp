// RendererConstantBufferService.cpp
#include "RendererConstantBufferService.hpp"
#include <d3dx12.h>
#include <stdexcept>

using namespace DirectX;

bool RendererConstantBufferService::Create(ID3D12Device* device) {
  CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
  auto cbDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(MVP) + 255) & ~255);

  if (FAILED(device->CreateCommittedResource(
      &heapProps,
      D3D12_HEAP_FLAG_NONE,
      &cbDesc,
      D3D12_RESOURCE_STATE_GENERIC_READ,
      nullptr,
      IID_PPV_ARGS(&constantBuffer)))) {
    return false;
      }

  cbAddress = constantBuffer->GetGPUVirtualAddress();
  return true;
}

void RendererConstantBufferService::UpdateModelMatrix(
    const XMMATRIX& model,
    const XMMATRIX& view,
    const XMMATRIX& projection) const
{
  MVP mvp;
  mvp.model = model;
  mvp.view = view;
  mvp.projection = projection;

  void* cbPtr = nullptr;
  D3D12_RANGE readRange{0, 0};
  if (SUCCEEDED(constantBuffer->Map(0, &readRange, &cbPtr))) {
    memcpy(cbPtr, &mvp, sizeof(MVP));
    constantBuffer->Unmap(0, nullptr);
  }
}

D3D12_GPU_VIRTUAL_ADDRESS RendererConstantBufferService::GetGPUAddress() const {
  return cbAddress;
}

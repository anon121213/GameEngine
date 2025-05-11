#include "RendererConstantBufferService.hpp"
#include <d3dx12.h>
#include <stdexcept>

using namespace DirectX;

bool RendererConstantBufferService::Create(ID3D12Device* device) {
  CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
  auto bufferSize = sizeof(MVP) * MaxObjects;
  auto desc = CD3DX12_RESOURCE_DESC::Buffer((bufferSize + 255) & ~255);

  if (FAILED(device->CreateCommittedResource(
      &heapProps,
      D3D12_HEAP_FLAG_NONE,
      &desc,
      D3D12_RESOURCE_STATE_GENERIC_READ,
      nullptr,
      IID_PPV_ARGS(&constantBuffer)))) {
    return false;
      }

  baseAddress = constantBuffer->GetGPUVirtualAddress();
  return true;
}

void RendererConstantBufferService::UpdateModelMatrix(uint32_t index, const XMMATRIX& model, const XMMATRIX& view, const XMMATRIX& proj) const {
  MVP mvp = { model, view, proj };

  void* ptr = nullptr;
  D3D12_RANGE readRange{0, 0};
  if (SUCCEEDED(constantBuffer->Map(0, &readRange, &ptr))) {
    auto offset = index * sizeof(MVP);
    memcpy(static_cast<uint8_t*>(ptr) + offset, &mvp, sizeof(MVP));
    constantBuffer->Unmap(0, nullptr);
  }
}

D3D12_GPU_VIRTUAL_ADDRESS RendererConstantBufferService::GetGPUAddress(uint32_t index) const {
  return baseAddress + index * sizeof(MVP);
}

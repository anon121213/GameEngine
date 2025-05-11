#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>

class RendererConstantBufferService {
public:
  static constexpr size_t MaxObjects = 256;

  bool Create(ID3D12Device* device);
  void UpdateModelMatrix(uint32_t index, const DirectX::XMMATRIX& model,
                         const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection) const;
  D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress(uint32_t index) const;

private:
  struct alignas(256) MVP {
    DirectX::XMMATRIX model;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

  Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;
  D3D12_GPU_VIRTUAL_ADDRESS baseAddress = 0;
};

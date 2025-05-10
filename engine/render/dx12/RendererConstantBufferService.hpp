#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>

class RendererConstantBufferService {
public:
  bool Create(ID3D12Device* device);
  void UpdateModelMatrix(const DirectX::XMMATRIX& model, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) const;
  D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() const;

private:
  struct alignas(256) MVP {
    DirectX::XMMATRIX model;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  } mvp{};

  Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;
  D3D12_GPU_VIRTUAL_ADDRESS cbAddress;
};
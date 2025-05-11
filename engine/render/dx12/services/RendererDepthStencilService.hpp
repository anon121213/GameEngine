#pragma once
#include <d3d12.h>
#include <wrl.h>

class RendererDepthStencilService {
public:
  bool Create(ID3D12Device* device, int width, int height);
  D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const;
  ID3D12Resource* GetBuffer() const;

private:
  Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilBuffer;
  Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
};

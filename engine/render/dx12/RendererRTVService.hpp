#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <array>

#include "RendererSwapChainService.hpp"

class RendererRTVService {
public:
  static constexpr UINT FrameCount = 2;

  bool Create(ID3D12Device* device, IDXGISwapChain3* swapChain);
  void UpdateFrameIndex(UINT newIndex);

  D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle() const;
  UINT GetDescriptorSize() const;
  ID3D12Resource* GetCurrentRenderTarget() const;

private:
  Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
  std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, FrameCount> renderTargets;

  UINT rtvDescriptorSize = 0;
  UINT currentFrameIndex = 0;
};

#pragma once
#include <dxgi1_6.h>
#include <wrl.h>

#include "RendererCommandService.hpp"

class RendererSwapChainService {
public:
  bool Create(ID3D12CommandQueue* queue, HWND hwnd, int width, int height);
  void Present() const;
  void UpdateFrameIndex();
  UINT GetCurrentFrameIndex() const;

  IDXGISwapChain3* GetSwapChain();

private:
  Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;
  UINT currentFrameIndex = 0;
};

#include "RendererSwapChainService.hpp"
#include <cassert>

bool RendererSwapChainService::Create(ID3D12CommandQueue* commandQueue, HWND hwnd, int width, int height) {
  Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
  if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) return false;

  DXGI_SWAP_CHAIN_DESC1 swapDesc{};
  swapDesc.BufferCount = 2;
  swapDesc.Width = width;
  swapDesc.Height = height;
  swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapDesc.SampleDesc.Count = 1;

  Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain;
  if (FAILED(factory->CreateSwapChainForHwnd(commandQueue, hwnd, &swapDesc, nullptr, nullptr, &tempSwapChain)))
    return false;

  if (FAILED(tempSwapChain.As(&swapChain))) return false;

  UpdateFrameIndex();
  return true;
}

void RendererSwapChainService::Present() const {
  assert(swapChain);
  swapChain->Present(1, 0);
}

void RendererSwapChainService::UpdateFrameIndex() {
  if (swapChain) {
    currentFrameIndex = swapChain->GetCurrentBackBufferIndex();
  }
}

UINT RendererSwapChainService::GetCurrentFrameIndex() const {
  return currentFrameIndex;
}

IDXGISwapChain3* RendererSwapChainService::GetSwapChain() {
  return swapChain.Get();
}

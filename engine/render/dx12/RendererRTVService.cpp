#include "RendererRTVService.hpp"
#include <d3dx12.h>

#include "RendererSwapChainService.hpp"

bool RendererRTVService::Create(ID3D12Device* device, IDXGISwapChain3* swapChain) {
  D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
  heapDesc.NumDescriptors = FrameCount;
  heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

  if (FAILED(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap)))) {
    return false;
  }

  rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());

  for (UINT i = 0; i < FrameCount; ++i) {
    if (FAILED(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])))) {
      return false;
    }
    device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
    rtvHandle.Offset(1, rtvDescriptorSize);
  }

  return true;
}

void RendererRTVService::UpdateFrameIndex(UINT newFrameIndex) {
  currentFrameIndex = newFrameIndex;
}

D3D12_CPU_DESCRIPTOR_HANDLE RendererRTVService::GetRTVHandle() const {
  CD3DX12_CPU_DESCRIPTOR_HANDLE handle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), currentFrameIndex, rtvDescriptorSize);
  return handle;
}

ID3D12Resource* RendererRTVService::GetCurrentRenderTarget() const {
  return renderTargets[currentFrameIndex].Get();
}

UINT RendererRTVService::GetDescriptorSize() const {
  return rtvDescriptorSize;
}
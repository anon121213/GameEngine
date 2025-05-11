#pragma once
#include <wrl.h>
#include <d3d12.h>

#include "render/components/RenderMeshComponent.hpp"

class RendererCommandService {
public:
  bool CreateDevice();
  bool CreateCommandObjects();

  void BeginFrame(ID3D12PipelineState* pipeline) const;
  void EndFrame(ID3D12Resource* renderTarget) const;

  void PrepareRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle,UINT rtvDescriptorSize,ID3D12Resource* currentRenderTarget,int width,int height,D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) const;
  void SetGraphicsState(ID3D12RootSignature* rootSig, D3D12_GPU_VIRTUAL_ADDRESS cbAddress, ID3D12PipelineState* pso) const;
  void DrawMesh(const RenderMeshComponent& mesh) const;

  ID3D12Device* GetDevice() const;
  ID3D12CommandQueue* GetCommandQueue() const;
  ID3D12GraphicsCommandList* GetCommandList() const;

private:
  Microsoft::WRL::ComPtr<ID3D12Device> device;
  Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
  Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
};

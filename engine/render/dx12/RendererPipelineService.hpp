#pragma once
#include <d3d12.h>
#include <wrl.h>

class RendererPipelineService {
public:
  bool Create(ID3D12Device* device, ID3D12RootSignature* rootSignature);
  ID3D12PipelineState* GetPipelineState() const;

private:
  Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

#pragma once
#include <d3d12.h>
#include <wrl.h>

class RendererRootSignatureService {
public:
  bool Create(ID3D12Device* device);
  ID3D12RootSignature* Get() const;

private:
  Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
};

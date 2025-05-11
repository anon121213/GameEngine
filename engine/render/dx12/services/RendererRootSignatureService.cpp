#include "RendererRootSignatureService.hpp"
#include <d3dx12.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

bool RendererRootSignatureService::Create(ID3D12Device* device) {
  CD3DX12_ROOT_PARAMETER rootParam;
  rootParam.InitAsConstantBufferView(0);

  CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
  rootSigDesc.Init(1, &rootParam, 0, nullptr,
                   D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

  ComPtr<ID3DBlob> serializedRootSig = nullptr;
  ComPtr<ID3DBlob> errorBlob = nullptr;

  HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
                                           &serializedRootSig, &errorBlob);

  if (FAILED(hr)) {
    if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    return false;
  }

  hr = device->CreateRootSignature(0,
                                   serializedRootSig->GetBufferPointer(),
                                   serializedRootSig->GetBufferSize(),
                                   IID_PPV_ARGS(&rootSignature));

  return SUCCEEDED(hr);
}

ID3D12RootSignature* RendererRootSignatureService::Get() const {
  return rootSignature.Get();
}

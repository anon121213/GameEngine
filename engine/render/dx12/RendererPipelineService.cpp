#include "RendererPipelineService.hpp"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <wrl.h>
#include <stdexcept>

using Microsoft::WRL::ComPtr;

bool RendererPipelineService::Create(ID3D12Device* device, ID3D12RootSignature* rootSig) {
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> psBlob;

    if (FAILED(D3DReadFileToBlob(L"shaders/VertexShader.cso", &vsBlob)) ||
        FAILED(D3DReadFileToBlob(L"shaders/PixelShader.cso", &psBlob))) {
        MessageBoxA(nullptr, "Failed to load shader .cso files", "DX12", MB_OK | MB_ICONERROR);
        return false;
    }

    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
    psoDesc.pRootSignature = rootSig;
    psoDesc.VS = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };
    psoDesc.PS = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC{};
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));
    if (FAILED(hr)) {
        MessageBoxA(nullptr, "Failed to create pipeline state", "DX12", MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

ID3D12PipelineState* RendererPipelineService::GetPipelineState() const {
    return pipelineState.Get();
}

#include "RendererPipelineService.hpp"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <wrl.h>
#include <stdexcept>
#include <filesystem>
#include <string>
#include <windows.h>

using Microsoft::WRL::ComPtr;
namespace fs = std::filesystem;

bool RendererPipelineService::Create(ID3D12Device* device, ID3D12RootSignature* rootSig) {
    ComPtr<ID3DBlob> vsBlob;
    ComPtr<ID3DBlob> psBlob;

    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    fs::path exeDir = fs::path(exePath).parent_path();

    fs::path shaderDir = exeDir / "assets" / "shaders";
    fs::path vertexShaderPath = shaderDir / "VertexShader.cso";
    fs::path pixelShaderPath = shaderDir / "PixelShader.cso";

    std::wstring debugMsg =
        L"Trying to load shaders from:\n\n" +
        vertexShaderPath.wstring() + L"\n" +
        pixelShaderPath.wstring() + L"\n\nCurrent exe dir:\n" +
        exeDir.wstring();

    if (FAILED(D3DReadFileToBlob(vertexShaderPath.c_str(), &vsBlob)) ||
        FAILED(D3DReadFileToBlob(pixelShaderPath.c_str(), &psBlob))) {

        MessageBoxA(nullptr, "Failed to load shader .cso files", "Pipeline", MB_OK | MB_ICONERROR);
        return false;
    }

    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
    psoDesc.pRootSignature = rootSig;
    psoDesc.VS = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };
    psoDesc.PS = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };

    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    psoDesc.RasterizerState.FrontCounterClockwise = FALSE;

    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    psoDesc.SampleDesc.Count = 1;

    return SUCCEEDED(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)));
}

ID3D12PipelineState* RendererPipelineService::GetPipelineState() const {
    return pipelineState.Get();
}

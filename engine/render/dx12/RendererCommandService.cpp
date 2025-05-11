// CommandService.cpp
#include "RendererCommandService.hpp"
#include "render/components/RenderMeshComponent.hpp"
#include <d3dx12.h>

bool RendererCommandService::CreateDevice() {
    return SUCCEEDED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
}

bool RendererCommandService::CreateCommandObjects() {
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)))) return false;
    if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)))) return false;
    if (FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)))) return false;
    return true;
}

void RendererCommandService::BeginFrame(ID3D12PipelineState* pipelineState) const {
    commandAllocator->Reset();
    commandList->Reset(commandAllocator.Get(), pipelineState);
}

void RendererCommandService::PrepareRenderTarget(
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle,
    UINT rtvDescriptorSize,
    ID3D12Resource* currentRenderTarget,
    int width,
    int height,
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) const
{
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        currentRenderTarget,
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList->ResourceBarrier(1, &barrier);

    D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f };
    D3D12_RECT scissorRect = { 0, 0, width, height };
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

    const float clearColor[] = { 0.2f, 0.3f, 0.4f, 1.0f };
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


void RendererCommandService::SetGraphicsState(ID3D12RootSignature* rootSignature, UINT64 cbAddress, ID3D12PipelineState* pipelineState) const {
    commandList->SetGraphicsRootSignature(rootSignature);
    commandList->SetGraphicsRootConstantBufferView(0, cbAddress);
    commandList->SetPipelineState(pipelineState);
}


void RendererCommandService::EndFrame(ID3D12Resource* renderTarget) const {
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    commandList->ResourceBarrier(1, &barrier);
    commandList->Close();
    ID3D12CommandList* lists[] = {commandList.Get()};
    commandQueue->ExecuteCommandLists(1, lists);
}

void RendererCommandService::DrawMesh(const RenderMeshComponent& mesh) const {
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);

    if (!mesh.indices.empty()) {
        commandList->IASetIndexBuffer(&mesh.indexBufferView);
        commandList->DrawIndexedInstanced(static_cast<UINT>(mesh.indices.size()), 1, 0, 0, 0);
    } else {
        commandList->DrawInstanced(static_cast<UINT>(mesh.vertices.size()), 1, 0, 0);
    }
}

ID3D12Device* RendererCommandService::GetDevice() const { return device.Get(); }
ID3D12CommandQueue* RendererCommandService::GetCommandQueue() const { return commandQueue.Get(); }
ID3D12GraphicsCommandList* RendererCommandService::GetCommandList() const { return commandList.Get(); }

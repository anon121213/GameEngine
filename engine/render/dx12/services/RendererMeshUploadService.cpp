#include "RendererMeshUploadService.hpp"
#include <d3dx12.h>
#include <cstring>

using Microsoft::WRL::ComPtr;

void RendererMeshUploadService::UploadIfNeeded(ID3D12Device* device, RenderMeshComponent& mesh) {
    if (mesh.initialized) return;

    CreateVertexBuffer(device, mesh);

    if (!mesh.indices.empty()) {
        CreateIndexBuffer(device, mesh);
    }

    mesh.initialized = true;
}

void RendererMeshUploadService::CreateVertexBuffer(ID3D12Device* device, RenderMeshComponent& mesh) {
    UINT vertexBufferSize = static_cast<UINT>(mesh.vertices.size() * sizeof(Vertex));
    auto vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RANGE readRange(0, 0);

    device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &vbDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mesh.vertexBuffer)
    );

    void* vbData = nullptr;
    mesh.vertexBuffer->Map(0, &readRange, &vbData);
    std::memcpy(vbData, mesh.vertices.data(), vertexBufferSize);
    mesh.vertexBuffer->Unmap(0, nullptr);

    mesh.vertexBufferView.BufferLocation = mesh.vertexBuffer->GetGPUVirtualAddress();
    mesh.vertexBufferView.StrideInBytes = sizeof(Vertex);
    mesh.vertexBufferView.SizeInBytes = vertexBufferSize;
}

void RendererMeshUploadService::CreateIndexBuffer(ID3D12Device* device, RenderMeshComponent& mesh) {
    UINT indexBufferSize = static_cast<UINT>(mesh.indices.size() * sizeof(uint32_t));
    auto ibDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RANGE readRange(0, 0);

    device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &ibDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&mesh.indexBuffer)
    );

    void* ibData = nullptr;
    mesh.indexBuffer->Map(0, &readRange, &ibData);
    std::memcpy(ibData, mesh.indices.data(), indexBufferSize);
    mesh.indexBuffer->Unmap(0, nullptr);

    mesh.indexBufferView.BufferLocation = mesh.indexBuffer->GetGPUVirtualAddress();
    mesh.indexBufferView.SizeInBytes = indexBufferSize;
    mesh.indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

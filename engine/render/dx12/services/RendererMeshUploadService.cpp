#include "render/dx12/services/RendererMeshUploadService.hpp"
#include <d3dx12.h>
#include <cstring>
#include "assets/MeshManager.hpp"
#include "services/ServiceLocator.hpp"
#include "core/Log.hpp"

using Microsoft::WRL::ComPtr;

void RendererMeshUploadService::UploadIfNeeded(ID3D12Device* device, SubMesh& subMesh)
{
    if (!device)
    {
        LOG_ERROR("RendererMeshUploadService::UploadIfNeeded — device is null!");
        return;
    }

    if (subMesh.initialized)
        return;

    if (subMesh.meshPath.empty())
    {
        LOG_ERROR("RendererMeshUploadService::UploadIfNeeded — meshPath is empty!");
        return;
    }

    MeshAsset asset = ServiceLocator::Get<MeshManager>()->LoadMesh(subMesh.meshPath);

    if (asset.vertices.empty())
    {
        LOG_ERROR("RendererMeshUploadService::UploadIfNeeded — asset {} has no vertices!", subMesh.meshPath);
        return;
    }

    CreateVertexBuffer(device, subMesh, asset);

    if (!asset.indices.empty())
        CreateIndexBuffer(device, subMesh, asset);

    subMesh.initialized = true;
    LOG_INFO("SubMesh uploaded to GPU: {} ({} vertices, {} indices)",
        subMesh.meshPath, asset.vertices.size(), asset.indices.size());
}

void RendererMeshUploadService::CreateVertexBuffer(ID3D12Device* device, SubMesh& subMesh, const MeshAsset& asset)
{
    const UINT vbSize = static_cast<UINT>(asset.vertices.size() * sizeof(Vertex));

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vbSize);

    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&subMesh.vertexBuffer)
    );

    if (FAILED(hr))
    {
        LOG_ERROR("Failed to create vertex buffer for subMesh: {}", subMesh.meshPath);
        return;
    }

    void* vbData = nullptr;
    D3D12_RANGE readRange = {0, 0};
    hr = subMesh.vertexBuffer->Map(0, &readRange, &vbData);
    if (SUCCEEDED(hr))
    {
        std::memcpy(vbData, asset.vertices.data(), vbSize);
        subMesh.vertexBuffer->Unmap(0, nullptr);
    }
    else
    {
        LOG_ERROR("Failed to map vertex buffer for subMesh: {}", subMesh.meshPath);
    }

    subMesh.vertexBufferView.BufferLocation = subMesh.vertexBuffer->GetGPUVirtualAddress();
    subMesh.vertexBufferView.StrideInBytes = sizeof(Vertex);
    subMesh.vertexBufferView.SizeInBytes = vbSize;
}

void RendererMeshUploadService::CreateIndexBuffer(ID3D12Device* device, SubMesh& subMesh, const MeshAsset& asset)
{
    const UINT ibSize = static_cast<UINT>(asset.indices.size() * sizeof(uint32_t));

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(ibSize);

    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&subMesh.indexBuffer)
    );

    if (FAILED(hr))
    {
        LOG_ERROR("Failed to create index buffer for subMesh: {}", subMesh.meshPath);
        return;
    }

    void* ibData = nullptr;
    D3D12_RANGE readRange = {0, 0};
    hr = subMesh.indexBuffer->Map(0, &readRange, &ibData);
    if (SUCCEEDED(hr))
    {
        std::memcpy(ibData, asset.indices.data(), ibSize);
        subMesh.indexBuffer->Unmap(0, nullptr);
    }
    else
    {
        LOG_ERROR("Failed to map index buffer for subMesh: {}", subMesh.meshPath);
    }

    subMesh.indexBufferView.BufferLocation = subMesh.indexBuffer->GetGPUVirtualAddress();
    subMesh.indexBufferView.SizeInBytes = ibSize;
    subMesh.indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

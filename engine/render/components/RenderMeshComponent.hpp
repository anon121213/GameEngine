#pragma once
#include <vector>
#include <wrl/client.h>
#include <d3d12.h>
#include <DirectXMath.h>

struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 color;
};

struct RenderMeshComponent {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
    D3D12_INDEX_BUFFER_VIEW indexBufferView{};

    bool initialized = false;
};

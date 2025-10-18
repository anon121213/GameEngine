#pragma once
#include <string>
#include <vector>
#include <wrl/client.h>
#include <d3d12.h>
#include <DirectXMath.h>

struct SubMesh {
    std::string meshPath;
    std::string materialPath;

    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
    D3D12_INDEX_BUFFER_VIEW indexBufferView{};
    bool initialized = false;
};

struct RenderMeshComponent {
    std::vector<SubMesh> subMeshes;
    uint32_t constantBufferIndex = 0;
};

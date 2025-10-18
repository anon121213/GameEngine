#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 position{};
    DirectX::XMFLOAT3 normal{};
    DirectX::XMFLOAT3 color{};
};

struct MeshAsset
{
    std::string path;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

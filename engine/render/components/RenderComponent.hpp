#pragma once
#include <DirectXMath.h>

struct RenderComponent {
    // Пока просто трансформация и заглушки под future mesh/material
    DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
    
    // Потом:
    // MeshHandle mesh;
    // MaterialHandle material;
};
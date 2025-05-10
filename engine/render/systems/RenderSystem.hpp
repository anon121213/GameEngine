#pragma once
#include "ecs/ISystem.hpp"
#include "render/dx12/DX12Renderer.hpp"

class RenderSystem : public ISystem {
public:
    void OnInitialize() override;
    void OnStart() override {}
    void OnUpdate() override;
    void OnFixedUpdate() override {}
    void OnLateUpdate() override {}
    void OnDispose() override {}
    
private:
    std::shared_ptr<DX12Renderer> renderer;
};

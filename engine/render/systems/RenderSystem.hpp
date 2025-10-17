#pragma once
#include "ecs/ISystem.hpp"
#include "render/dx12/DX12Renderer.hpp"

class RenderSystem final : public ISystem {
public:
    void OnUpdate(float deltaTime) override;
    
private:
    std::shared_ptr<DX12Renderer> renderer = ServiceLocator::Get<DX12Renderer>();;
};

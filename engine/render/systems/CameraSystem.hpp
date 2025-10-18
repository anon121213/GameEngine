#pragma once
#include "components/base/Transform.hpp"
#include "ecs/ISystem.hpp"
#include "components/rendering/CameraComponent.hpp"
#include "render/dx12/DX12Renderer.hpp"

class CameraSystem final : public ISystem {
public:
  void OnUpdate(float deltaTime) override;

private:
  std::shared_ptr<DX12Renderer> renderer = ServiceLocator::Get<DX12Renderer>();;
  static DirectX::XMMATRIX GetRotationMatrix(const Transform &t);
};

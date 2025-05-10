#pragma once
#include "components/Transform.hpp"
#include "ecs/ISystem.hpp"
#include "render/components/CameraComponent.hpp"
#include "render/dx12/DX12Renderer.hpp"

class CameraSystem : public ISystem {
public:
  void OnInitialize() override;
  void OnUpdate() override;

private:
  std::shared_ptr<DX12Renderer> renderer;

  static DirectX::XMMATRIX GetRotationMatrix(const Transform &t);

public:
  void OnStart() override {}

  void OnFixedUpdate() override {}

  void OnLateUpdate() override {}

  void OnDispose() override {}
};

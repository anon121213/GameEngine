#include "CameraSystem.hpp"
#include "services/ServiceLocator.hpp"
#include "ecs/World.hpp"
using namespace DirectX;

void CameraSystem::OnInitialize() {
  renderer = GET_SERVICE(DX12Renderer);
}

void CameraSystem::OnUpdate() {
  for (auto [entity, transform, camera] : world->View<Transform, CameraComponent>()) {
    if (!camera.primary) continue;

    XMMATRIX view = transform.GetViewMatrix();
    XMMATRIX proj = XMMatrixPerspectiveFovLH(
        camera.fov, renderer->GetAspectRatio(), camera.nearZ, camera.farZ);

    renderer->SetViewProjection(view, proj);
  }
}

XMMATRIX CameraSystem::GetRotationMatrix(const Transform& t) {
  return XMMatrixRotationRollPitchYaw(t.rotation.x, t.rotation.y, t.rotation.z);
}

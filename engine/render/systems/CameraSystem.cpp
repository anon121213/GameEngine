#include "CameraSystem.hpp"
#include "services/ServiceLocator.hpp"
#include "ecs/World.hpp"
using namespace DirectX;

void CameraSystem::OnInitialize() {
  renderer = GET_SERVICE(DX12Renderer);
}

void CameraSystem::OnUpdate() {
  for (auto [entity, transform, camera] : world->View<Transform, CameraComponent>()) {
    XMVECTOR eyePos = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&transform.position));
    XMVECTOR forward = XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), GetRotationMatrix(transform));
    XMVECTOR up = XMVectorSet(0, 1, 0, 0);

    XMMATRIX view = XMMatrixLookToLH(eyePos, forward, up);
    XMMATRIX proj = XMMatrixPerspectiveFovLH(camera.fov, renderer->GetAspectRatio(), camera.nearPlane, camera.farPlane);

    renderer->SetViewProjection(view, proj);
  }
}

XMMATRIX CameraSystem::GetRotationMatrix(const Transform& t) {
  return XMMatrixRotationRollPitchYaw(t.rotation.x, t.rotation.y, t.rotation.z);
}

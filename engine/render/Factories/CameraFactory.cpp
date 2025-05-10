#include "CameraFactory.hpp"

Entity CameraFactory::CreatePrimaryCamera(std::shared_ptr<World> world) {
  Entity cameraEntity = world->CreateEntity();
  Transform transform;
  transform.position = { 0.0f, 0.0f, -3.0f };

  CameraComponent camera;
  camera.primary = true;
  camera.fov = DirectX::XM_PIDIV4;
  camera.nearZ = 0.1f;
  camera.farZ = 100.0f;

  world->AddComponent(cameraEntity, transform);
  world->AddComponent(cameraEntity, camera);
  return cameraEntity;
}

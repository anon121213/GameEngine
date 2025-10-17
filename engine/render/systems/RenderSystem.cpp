#include "render/systems/RenderSystem.hpp"

#include "components/Transform.hpp"
#include "render/components/RenderMeshComponent.hpp"

void RenderSystem::OnUpdate(float deltaTime) {
  for (auto [entity, transform, mesh]: world->View<Transform, RenderMeshComponent>()) {
    renderer->DrawMesh(mesh, transform);
  }
}

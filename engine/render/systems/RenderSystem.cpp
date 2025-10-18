#include "render/systems/RenderSystem.hpp"

#include "components/base/Transform.hpp"
#include "components/rendering/RenderMeshComponent.hpp"

void RenderSystem::OnUpdate(float deltaTime) {
  for (auto [entity, transform, mesh]: world->View<Transform, RenderMeshComponent>()) {
    renderer->DrawMesh(mesh, transform);
  }
}

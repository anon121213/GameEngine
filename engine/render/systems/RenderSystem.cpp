#include "render/systems/RenderSystem.hpp"

#include "components/Transform.hpp"
#include "render/components/RenderMeshComponent.hpp"

void RenderSystem::OnInitialize() {
  renderer = GET_SERVICE(DX12Renderer);
}

void RenderSystem::OnUpdate() {
  for (auto [entity, transform, mesh]: world->View<Transform, RenderMeshComponent>()) {
    renderer->DrawMesh(mesh, transform);
  }
}

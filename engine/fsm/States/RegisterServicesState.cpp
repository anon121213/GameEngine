#include <render/dx12/managers/Mesh/MeshManager.hpp>

#include "RegisterServicesState.hpp"
#include "components/base/Transform.hpp"
#include "render/RenderService.hpp"
#include "components/rendering/CameraComponent.hpp"
#include "render/dx12/services/ModelLoaderService.hpp"
#include "render/Factories/RenderObjectFactory.hpp"
#include "render/systems/CameraSystem.hpp"
#include "render/systems/RenderSystem.hpp"
#include "services/ServiceLocator.hpp"
#include "ecs/SystemsContainer.hpp"
#include "fsm/StateMachine.hpp"
#include "fsm/states/StartEngineState.hpp"

void RegisterServicesState::Enter(){
  // ECS
  ServiceLocator::Register<World>(std::make_shared<World>());
  ServiceLocator::Register<SystemsContainer>(std::make_shared<SystemsContainer>());  
  
  // RENDER
  ServiceLocator::Register<DX12Renderer>(std::make_shared<DX12Renderer>());
  ServiceLocator::Register<RenderService>(std::make_shared<RenderService>());
  ServiceLocator::Register<MeshManager>(std::make_shared<MeshManager>());
  ServiceLocator::Register<ModelLoaderService>(std::make_shared<ModelLoaderService>());
  
  const auto fsm = ServiceLocator::Get<StateMachine>();
  fsm->ChangeState<StartEngineState>();
}

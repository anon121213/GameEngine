#pragma once
#include "ecs/World.hpp"
#include "components/Transform.hpp"
#include "render/components/CameraComponent.hpp"

class CameraFactory {
public:
  static Entity CreatePrimaryCamera(std::shared_ptr<World> world);
};
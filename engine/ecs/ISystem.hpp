#pragma once
#include "World.hpp"
#include "../services/ServiceLocator.hpp"
#include "core/Log.hpp"

class ISystem {
public:
    virtual ~ISystem() = default;

    virtual void OnInitialize() {}
    virtual void OnStart() {}
    virtual void OnFixedUpdate(float fixedDeltaTime) {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnLateUpdate(float deltaTime) {}
    virtual void OnDispose() {}

protected:
    std::shared_ptr<World> world = ServiceLocator::Get<World>();
};

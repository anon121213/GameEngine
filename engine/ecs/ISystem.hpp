#pragma once
#include "World.hpp"
#include "../services/ServiceLocator.hpp"

class ISystem {
public:
    virtual ~ISystem() = default;

    void Initialize() {
        world = ServiceLocator::Get<World>();
        OnInitialize();
    }

    virtual void OnInitialize() {}
    virtual void OnStart() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnFixedUpdate() = 0;
    virtual void OnLateUpdate() = 0;
    virtual void OnDispose() = 0;

protected:
    std::shared_ptr<World> world;
};

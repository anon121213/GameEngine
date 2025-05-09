#pragma once

#include <memory>
#include "ecs/World.hpp"
#include "render/RenderService.hpp"

class RenderService;

class Engine {
public:
    Engine();
    void Awake();
    void Start();
    void FixedUpdate();
    void Update(float deltaTime);
    void LateUpdate();
    void Shutdown();

    void RunLoop();

private:
    bool running = true;
    float fixedTimeStep = 1.0f / 60.0f;
    std::unique_ptr<World> world;
    std::unique_ptr<RenderService> renderService;
};

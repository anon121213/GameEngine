#pragma once

#include <memory>

#include "ecs/SystemsContainer.hpp"
#include "render/RenderService.hpp"

class RenderService;

class Engine {
public:
    Engine();
    
    void RunEditorLoop(HINSTANCE hInstance);

private:
    bool running = true;
    float fixedTimeStep = 1.0f / 60.0f;

    std::shared_ptr<World> world;
    std::unique_ptr<RenderService> renderService;
    std::unique_ptr<SystemsContainer> systemsContainer;
    
    void Awake();

    void Start();

    void FixedUpdate();

    void Update(float deltaTime);

    void LateUpdate();

    void Shutdown();
};

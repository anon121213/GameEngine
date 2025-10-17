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
    float fixedTimeStep = 1.0f / 60.0f;float totalTime = 0.0f;

    std::shared_ptr<World> world = ServiceLocator::Get<World>();
    std::shared_ptr<RenderService> renderService = ServiceLocator::Get<RenderService>();
    std::shared_ptr<SystemsContainer> systemsContainer = ServiceLocator::Get<SystemsContainer>();
    
    void Awake() const;

    void Start() const;

    void FixedUpdate(float fixedDeltaTime) const;

    void Update(float deltaTime);

    void LateUpdate(float deltaTime) const;

    void Dispose();
};

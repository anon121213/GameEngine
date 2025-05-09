#include "Engine.hpp"
#include "core/Log.hpp"
#include "core/Window.hpp"
#include <chrono>

#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "render/RenderService.hpp"
#include "ecs/EntityView.hpp" 

Engine::Engine() {
    world = std::make_unique<World>();
}

void Engine::Awake() {
    LOG_INFO("Engine: Awake");
    // future: service init, config load
}

void Engine::Start() {
    LOG_INFO("Engine: Start");
}

void Engine::FixedUpdate() {
    // TODO: physics or logic fixed-timestep
}

void Engine::Update(float dt) {
}

void Engine::LateUpdate() {
    // TODO: cleanup systems
}

void Engine::Shutdown() {
    LOG_INFO("Engine: Shutdown");
    world.reset();
}

void Engine::RunLoop() {
    using Clock = std::chrono::high_resolution_clock;
    auto lastTime = Clock::now();
    float accumulator = 0.0f;

    renderService->CreateWindow(1280, 720, "GameEngine");

    Awake();
    Start();

    while (!Window::ShouldClose()) {
        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;
        accumulator += dt;

        renderService->BeginFrame();   

        while (accumulator >= fixedTimeStep) {
            FixedUpdate();
            accumulator -= fixedTimeStep;
        }

        Update(dt);
        LateUpdate();
        renderService->EndFrame();

        Window::PollEvents();
    }

    Shutdown();
}


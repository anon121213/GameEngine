#include "Engine.hpp"
#include "core/Log.hpp"
#include "core/Window.hpp"
#include <chrono>

#include "components/Transform.hpp"
#include "render/RenderService.hpp"


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

void Engine::RunLoop(HINSTANCE hInstance) {
    renderService = std::make_unique<RenderService>();
    float accumulator = 0.0f;
    if (!renderService->Init(hInstance, 1280, 920, "GameEngine")) {
        LOG_ERROR("Failed to initialize RenderService.");
        return;
    }

    Awake();
    Start();

    MSG msg = {};
    while (msg.message != WM_QUIT) {
        // Windows message processing
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Engine logic + rendering
        using Clock = std::chrono::high_resolution_clock;
        static auto lastTime = Clock::now();
        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        renderService->BeginFrame();

        accumulator += dt;
        while (accumulator >= fixedTimeStep) {
            FixedUpdate();
            accumulator -= fixedTimeStep;
        }

        Update(dt);
        LateUpdate();

        renderService->EndFrame();
    }

    Shutdown();
}



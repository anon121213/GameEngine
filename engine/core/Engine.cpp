#include "Engine.hpp"
#include "core/Log.hpp"
#include "core/Window.hpp"
#include <chrono>

#include "components/Transform.hpp"
#include "render/RenderService.hpp"
#include "render/components/CameraComponent.hpp"
#include "render/systems/CameraSystem.hpp"
#include "render/systems/RenderSystem.hpp"
#include "services/ServiceLocator.hpp"


Engine::Engine() {
    world = std::make_shared<World>();
    renderService = std::make_unique<RenderService>();
    systemsContainer = std::make_unique<SystemsContainer>();
    
    ServiceLocator::Register<World>(world);
    ServiceLocator::Register<DX12Renderer>(std::make_shared<DX12Renderer>());

    systemsContainer->AddSystem<RenderSystem>();
    systemsContainer->AddSystem<CameraSystem>();
}

void Engine::Awake() const {
    LOG_INFO("Engine: Awake");
    systemsContainer->OnInitialize();

    Entity triangle = world->CreateEntity();

    RenderMeshComponent mesh;
    mesh.vertices = {
        {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},   // top - red
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // right - green
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // left - blue
    };

    mesh.indices = { 0, 1, 2 };

    world->AddComponent<RenderMeshComponent>(triangle, mesh);
    world->AddComponent<Transform>(triangle, {});  // identity transform

// Camera
    Entity camera = world->CreateEntity();
    world->AddComponent<Transform>(camera, {
        .position = {0.0f, 0.0f, -2.0f},
    });
    world->AddComponent<CameraComponent>(camera, {});
}

void Engine::Start() const {
    LOG_INFO("Engine: Start");
    systemsContainer->OnStart();
}

void Engine::FixedUpdate() const {
    systemsContainer->OnFixedUpdate();
}

void Engine::Update(float dt) {
    totalTime += dt;

    for (auto [entity, transform] : world->View<Transform>()) {
        if (world->HasComponent<CameraComponent>(entity))
            continue;
        
        transform.rotation.y = totalTime;
    }
    systemsContainer->OnUpdate();
}

void Engine::LateUpdate() const {
    systemsContainer->OnLateUpdate();
}

void Engine::Shutdown() {
    LOG_INFO("Engine: Shutdown");
    systemsContainer->OnDispose();
    world.reset();
}

void Engine::RunEditorLoop(HINSTANCE hInstance) {
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



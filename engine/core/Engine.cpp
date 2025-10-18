#include "Engine.hpp"
#include "core/Log.hpp"
#include <chrono>
#include <render/dx12/services/ModelLoaderService.hpp>

#include "render/RenderService.hpp"
#include "render/Factories/RenderObjectFactory.hpp"
#include "render/systems/CameraSystem.hpp"
#include "render/systems/RenderSystem.hpp"
#include "services/ServiceLocator.hpp"

Engine::Engine() {
    systemsContainer->AddSystem<RenderSystem>();
    systemsContainer->AddSystem<CameraSystem>();
}

void Engine::Awake() const {
    LOG_INFO("Engine: Awake");
    ServiceLocator::InitializeAll();
    systemsContainer->OnInitialize();

    // TO DO Delete this debug objects from this 
    
    const Entity camera = world->CreateEntity();
    world->AddComponent<Transform>(camera, RenderObjectFactory::CreateTransform({0.0f, 3.0f, -50.0f}));
    world->AddComponent<CameraComponent>(camera, RenderObjectFactory::CreateCamera());

    const Entity car = world->CreateEntity();
    world->AddComponent<RenderMeshComponent>(car, RenderObjectFactory::GetFBXMesh("E:\\Projects\\GameEngine\\assets\\Koenigsegg\\source\\car.fbx"));
    world->AddComponent<Transform>(car, RenderObjectFactory::CreateTransform({0,-1,0}, {},  {0.07f, 0.07f, 0.07f}));
}

void Engine::Start() const {
    LOG_INFO("Engine: Start");
    ServiceLocator::StartAll();
    systemsContainer->OnStart();
}

void Engine::FixedUpdate(const float fixedDeltaTime) const {
    ServiceLocator::FixedUpdateAll(fixedDeltaTime);
    systemsContainer->OnFixedUpdate(fixedDeltaTime);
}

void Engine::Update(const float deltaTime) {
    totalTime += deltaTime;

    for (auto [entity, transform] : world->View<Transform>()) {
        if (world->HasComponent<CameraComponent>(entity))
            continue;
        
        transform.rotation.y = totalTime;
    }

    ServiceLocator::UpdateAll(deltaTime);
    systemsContainer->OnUpdate(deltaTime);
}

void Engine::LateUpdate(const float deltaTime) const {
    ServiceLocator::UpdateAll(deltaTime);
    systemsContainer->OnLateUpdate(deltaTime);
}

void Engine::Dispose() {
    ServiceLocator::DisposeAll();
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
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        using Clock = std::chrono::high_resolution_clock;
        static auto lastTime = Clock::now();
        auto now = Clock::now();
        const float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        renderService->BeginFrame();

        accumulator += dt;
        while (accumulator >= fixedTimeStep) {
            FixedUpdate(accumulator);
            accumulator -= fixedTimeStep;
        }

        Update(dt);
        LateUpdate(dt);

        renderService->EndFrame();
    }

    Dispose();
}
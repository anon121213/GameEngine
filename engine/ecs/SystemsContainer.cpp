#include "SystemsContainer.hpp"

void SystemsContainer::OnInitialize() const {
    for (const auto& system: systems)
        system->OnInitialize();
}

void SystemsContainer::OnStart() const {
    for (const auto& system: systems)
        system->OnStart();
}

void SystemsContainer::OnFixedUpdate(const float fixedDeltaTime) const {
    for (const auto& system: systems)
        system->OnFixedUpdate(fixedDeltaTime);
}

void SystemsContainer::OnUpdate(const float deltaTime) const {
    for (const auto& system: systems)
        system->OnUpdate(deltaTime);
}

void SystemsContainer::OnLateUpdate(const float deltaTime) const {
    for (const auto& system: systems)
        system->OnLateUpdate(deltaTime);
}

void SystemsContainer::OnDispose() const {
    for (const auto& system: systems)
        system->OnDispose();
}

#include "SystemsContainer.hpp"

void SystemsContainer::OnInitialize() const {
    for (const auto& system: systems) {
        system->Initialize();
        system->OnInitialize();
    }
}

void SystemsContainer::OnStart() const {
    for (const auto& system: systems)
        system->OnStart();
}

void SystemsContainer::OnUpdate() const {
    for (const auto& system: systems)
        system->OnUpdate();
}

void SystemsContainer::OnFixedUpdate() const {
    for (const auto& system: systems)
        system->OnFixedUpdate();
}

void SystemsContainer::OnLateUpdate() const {
    for (const auto& system: systems)
        system->OnLateUpdate();
}

void SystemsContainer::OnDispose() const {
    for (const auto& system: systems)
        system->OnDispose();
}

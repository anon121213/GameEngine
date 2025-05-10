#pragma once
#include <vector>
#include <memory>
#include <type_traits>

#include "ISystem.hpp"

class SystemsContainer {
public:
    template<typename T, typename... Args>
    void AddSystem(Args&&... args) {
        static_assert(std::is_base_of_v<ISystem, T>, "T must derive from ISystem");

        for (const auto& sys : systems)
            if (dynamic_cast<T*>(sys.get())) return;

        systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    void RemoveSystem() {
        systems.erase(std::remove_if(systems.begin(), systems.end(),
            [](const std::unique_ptr<ISystem>& sys) {
                return dynamic_cast<T*>(sys.get()) != nullptr;
            }),
            systems.end());
    }

    void OnInitialize() const;
    void OnStart() const;
    void OnUpdate() const;
    void OnFixedUpdate() const;
    void OnLateUpdate() const;
    void OnDispose() const;

private:
    std::vector<std::unique_ptr<ISystem>> systems;
};

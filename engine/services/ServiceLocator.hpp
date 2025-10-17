#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <algorithm>
#include <type_traits>

#include "IInitializeble.hpp"
#include "IStartable.hpp"
#include "IFixedUpdateble.hpp"
#include "IUpdateable.hpp"
#include "ILateUpdatable.hpp"
#include "IDisposable.hpp"

class ServiceLocator {
    friend class Engine;

public:
    template<typename T>
    static void Register(std::shared_ptr<T> service) {
        auto& map = services();
        map[typeid(T)] = service;

        if constexpr (std::is_base_of_v<IInitializeble, T>)
            initializebles().push_back(std::static_pointer_cast<IInitializeble>(service));

        if constexpr (std::is_base_of_v<IStartable, T>)
            starteables().push_back(std::static_pointer_cast<IStartable>(service));

        if constexpr (std::is_base_of_v<IFixedUpdateble, T>)
            fixedUpdatables().push_back(std::static_pointer_cast<IFixedUpdateble>(service));

        if constexpr (std::is_base_of_v<IUpdatable, T>)
            updatables().push_back(std::static_pointer_cast<IUpdatable>(service));

        if constexpr (std::is_base_of_v<ILateUpdatable, T>)
            lateUpdatables().push_back(std::static_pointer_cast<ILateUpdatable>(service));

        if constexpr (std::is_base_of_v<IDisposable, T>)
            disposables().push_back(std::static_pointer_cast<IDisposable>(service));
    }

    template<typename T>
    static std::shared_ptr<T> Get() {
        if (const auto it = services().find(typeid(T)); it != services().end())
            return std::static_pointer_cast<T>(it->second);
        return nullptr;
    }

private:
    template<typename T>
    static void Unregister() {
        services().erase(typeid(T));
    }

    static void InitializeAll() {
        for (const auto &a : initializebles()) a->OnInitialize();
    }

    static void StartAll() {
        for (const auto &a : starteables()) a->OnStart();
    }

    static void FixedUpdateAll(float fdt) {
        for (const auto &u : fixedUpdatables()) u->OnFixedUpdate(fdt);
    }

    static void UpdateAll(float dt) {
        for (const auto &u : updatables()) u->OnUpdate(dt);
    }

    static void LateUpdateAll(float dt) {
        for (const auto &u : lateUpdatables()) u->OnLateUpdate(dt);
    }

    static void DisposeAll() {
        for (const auto &s : disposables()) s->OnDispose();
    }

    static std::unordered_map<std::type_index, std::shared_ptr<void>>& services() {
        static std::unordered_map<std::type_index, std::shared_ptr<void>> instance;
        return instance;
    }

    static std::vector<std::shared_ptr<IInitializeble>>& initializebles() {
        static std::vector<std::shared_ptr<IInitializeble>> v;
        return v;
    }

    static std::vector<std::shared_ptr<IStartable>>& starteables() {
        static std::vector<std::shared_ptr<IStartable>> v;
        return v;
    }

    static std::vector<std::shared_ptr<IFixedUpdateble>>& fixedUpdatables() {
        static std::vector<std::shared_ptr<IFixedUpdateble>> v;
        return v;
    }

    static std::vector<std::shared_ptr<IUpdatable>>& updatables() {
        static std::vector<std::shared_ptr<IUpdatable>> v;
        return v;
    }

    static std::vector<std::shared_ptr<ILateUpdatable>>& lateUpdatables() {
        static std::vector<std::shared_ptr<ILateUpdatable>> v;
        return v;
    }

    static std::vector<std::shared_ptr<IDisposable>>& disposables() {
        static std::vector<std::shared_ptr<IDisposable>> v;
        return v;
    }
};
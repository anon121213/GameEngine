#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Entity.hpp"
#include "ComponentStorage.hpp"

class World {
public:
    Entity CreateEntity() {
        return nextEntityId++;
    }

    template<typename T>
    void AddComponent(Entity entity, const T& component) {
        GetOrCreateStorage<T>().Add(entity, component);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return GetOrCreateStorage<T>().Get(entity);
    }

    template<typename T>
    bool HasComponent(Entity entity) const {
        auto storage = GetStorage<T>();
        return storage && storage->Has(entity);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        auto storage = GetStorage<T>();
        if (storage) storage->Remove(entity);
    }

private:
    Entity nextEntityId = 1;
    std::unordered_map<std::type_index, std::unique_ptr<BaseStorage>> storages;

    template<typename T>
    ComponentStorage<T>* GetStorage() const {
        auto it = storages.find(std::type_index(typeid(T)));
        if (it == storages.end()) return nullptr;
        return static_cast<ComponentStorage<T>*>(it->second.get());
    }

    template<typename T>
    ComponentStorage<T>& GetOrCreateStorage() {
        auto* existing = GetStorage<T>();
        if (existing) return *existing;

        auto storage = std::make_unique<ComponentStorage<T>>();
        auto* rawPtr = storage.get();
        storages[std::type_index(typeid(T))] = std::move(storage);
        return *rawPtr;
    }
};

#pragma once
#include <unordered_map>
#include "Entity.hpp"
#include "BaseStorage.hpp"

template<typename T>
class ComponentStorage : public BaseStorage  {
public:
    void Add(Entity entity, const T& component) {
        components[entity] = component;
    }

    T& Get(Entity entity) {
        return components.at(entity);
    }

    bool Has(Entity entity) const {
        return components.find(entity) != components.end();
    }

    void Remove(Entity entity) {
        components.erase(entity);
    }

private:
    std::unordered_map<Entity, T> components;
};
#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>

class ServiceLocator {
public:
    template<typename T>
    static void Register(std::shared_ptr<T> service) {
        services()[typeid(T)] = service;
    }

    template<typename T>
    static std::shared_ptr<T> Get() {
        auto it = services().find(typeid(T));
        if (it != services().end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    template<typename T>
    static void Unregister() {
        services().erase(typeid(T));
    }

private:
    static std::unordered_map<std::type_index, std::shared_ptr<void>>& services() {
        static std::unordered_map<std::type_index, std::shared_ptr<void>> instance;
        return instance;
    }
};

#define GET_SERVICE(Type) ([]() { \
LOG_INFO("Getting " #Type " from ServiceLocator"); \
return ServiceLocator::Get<Type>(); \
})()
#pragma once
#include <tuple>
#include "Entity.hpp"
#include "ComponentStorage.hpp"

template<typename... Ts>
class EntityView {
public:
    using StorageTuple = std::tuple<ComponentStorage<Ts>*...>;
    using FirstStorage = typename std::tuple_element<0, StorageTuple>::type;
    using MapType = typename std::remove_pointer_t<FirstStorage>::MapType;

    class Iterator {
    public:
        Iterator(StorageTuple storages,
                 typename MapType::const_iterator current,
                 typename MapType::const_iterator end)
            : storages(storages), current(current), end(end) {
            SkipToValid();
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        void operator++() {
            ++current;
            SkipToValid();
        }

        auto operator*() const {
            Entity entity = current->first;
            return std::tuple<Entity, Ts&...>{entity, GetComponent<Ts>(entity)...};
        }

    private:
        StorageTuple storages;
        typename MapType::const_iterator current;
        typename MapType::const_iterator end;

        template<typename T>
        T& GetComponent(Entity entity) const {
            auto* storage = std::get<ComponentStorage<T>*>(storages);
            return storage->Get(entity);
        }

        bool HasAll(Entity entity) const {
            return (... && std::get<ComponentStorage<Ts>*>(storages)->Has(entity));
        }

        void SkipToValid() {
            while (current != end && !HasAll(current->first)) {
                ++current;
            }
        }
    };

    explicit EntityView(StorageTuple storages)
        : storages(storages), smallestStorage(std::get<0>(storages)) {}

    Iterator begin() const {
        const auto& all = smallestStorage->GetAll();
        return Iterator(storages, all.begin(), all.end());
    }

    Iterator end() const {
        const auto& all = smallestStorage->GetAll();
        return Iterator(storages, all.end(), all.end());
    }

private:
    StorageTuple storages;
    std::remove_pointer_t<typename std::tuple_element<0, StorageTuple>::type>* smallestStorage;
};

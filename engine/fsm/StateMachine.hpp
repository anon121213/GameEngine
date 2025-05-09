#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "IState.hpp"

class StateMachine {
public:
    StateMachine() = default;

    template<typename T>
    void Register(std::unique_ptr<IState> state);

    template<typename T>
    void ChangeState();

private:
    std::unordered_map<std::type_index, std::unique_ptr<IState>> states;
    IState* current = nullptr;
};

template<typename T>
void StateMachine::Register(std::unique_ptr<IState> state) {
    states[std::type_index(typeid(T))] = std::move(state);
}

template<typename T>
void StateMachine::ChangeState() {
    auto it = states.find(std::type_index(typeid(T)));
    if (it == states.end()) return;

    if (current)
        current->Exit();

    current = it->second.get();
    current->Enter();
}
#pragma once
#include <windows.h>
#include "fsm/IState.hpp"

class BootstrapState : public IState {
public:
    explicit BootstrapState(StateMachine& fsm, HINSTANCE hInstance);
    void Enter() override;
    void Exit() override;

private:
    StateMachine& fsm;
};

#pragma once
#include "fsm/IState.hpp"

class BootstrapState : public IState {
public:
    BootstrapState(StateMachine& fsm);
    void Enter() override;
    void Exit() override;

private:
    StateMachine& fsm;
};

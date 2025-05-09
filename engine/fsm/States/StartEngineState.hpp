#pragma once
#include "fsm/IState.hpp"
#include "fsm/StateMachine.hpp"

class StartEngineState : public IState  {
public:
    explicit StartEngineState(StateMachine& fsm);
    void Enter() override;
    void Exit() override;

private:
    StateMachine& fsm;
};

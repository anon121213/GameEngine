#pragma once
#include "fsm/IState.hpp"

class StartEngineState : public IState  {
public:
    StartEngineState(StateMachine& fsm);
    void Enter() override;
    void Exit() override;

private:
    StateMachine& fsm;
};

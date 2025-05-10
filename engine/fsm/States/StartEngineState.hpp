#pragma once
#include "fsm/IState.hpp"
#include "fsm/StateMachine.hpp"
#include "render/dx12/DX12Renderer.hpp"

class StartEngineState : public IState  {
public:
    explicit StartEngineState(StateMachine& fsm, HINSTANCE hInstance);
    void Enter() override;
    void Exit() override;

private:
    StateMachine& fsm;
    HINSTANCE hInstance;
};

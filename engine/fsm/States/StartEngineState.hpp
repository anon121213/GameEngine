#pragma once
#include "fsm/IState.hpp"
#include "fsm/StateMachine.hpp"
#include "render/dx12/DX12Renderer.hpp"

class StartEngineState : public IState  {
public:
    explicit StartEngineState (HINSTANCE hinstance);
    void Enter() override;
    void Exit() override;
private:
    HINSTANCE hinstance;
};

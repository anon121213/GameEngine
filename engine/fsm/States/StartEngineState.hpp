#pragma once
#include "fsm/IState.hpp"
#include "render/dx12/DX12Renderer.hpp"

class StartEngineState final : public IState  {
public:
    explicit StartEngineState (HINSTANCE hinstance);
    void Enter() override;
private:
    HINSTANCE hinstance;
};

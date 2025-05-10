#pragma once
#include <windows.h>
#include "fsm/IState.hpp"

class BootstrapState : public IState {
public:
    void Enter() override;
    void Exit() override;
};

#pragma once

#include "fsm/IState.hpp"

class BootstrapState final : public IState {
public:
  void Enter() override;
};

#pragma once

#include "fsm/IState.hpp"

class RegisterServicesState final : public IState {
public:
  void Enter() override;
};

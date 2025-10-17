#pragma once

struct IUpdatable {
  virtual void OnUpdate(float deltaTime) = 0;
  virtual ~IUpdatable() = default;
};

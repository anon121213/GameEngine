#pragma once

struct IFixedUpdateble {
  virtual void OnFixedUpdate(float fixedDeltaTime) = 0;
  virtual ~IFixedUpdateble() = default;
};

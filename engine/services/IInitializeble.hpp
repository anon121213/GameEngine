#pragma once

struct IInitializeble {
  virtual void OnInitialize() = 0;
  virtual ~IInitializeble() = default;
};

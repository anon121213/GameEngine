#pragma once

struct IStartable{
  virtual void OnStart() = 0;
  virtual ~IStartable() = default;
};

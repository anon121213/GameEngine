#pragma once

struct IDisposable {
  virtual void OnDispose() = 0;
  virtual ~IDisposable() = default;
};

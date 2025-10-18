#pragma once
#include <DirectXMath.h>

struct CameraComponent {
  float fov = DirectX::XM_PIDIV4;
  float nearZ = 0.1f;
  float farZ = 100.0f;
  bool primary = false;
};

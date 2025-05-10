#pragma once
#include <DirectXMath.h>

struct CameraComponent {
  float fov = DirectX::XM_PIDIV4;
  float nearPlane = 0.1f;
  float farPlane = 100.0f;
};
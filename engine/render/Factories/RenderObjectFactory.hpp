#pragma once

#include "render/components/RenderMeshComponent.hpp"
#include "components/Transform.hpp"
#include "render/components/CameraComponent.hpp"
#include <glm/glm.hpp>

class RenderObjectFactory {
public:
  static RenderMeshComponent CreateTriangle();
  static RenderMeshComponent CreateCube();
  static RenderMeshComponent CreatePlane(float size = 1.0f);

  static Transform CreateTransform(const glm::vec3& position = {0, 0, 0},
                                   const glm::vec3& rotation = {0, 0, 0},
                                   const glm::vec3& scale = {1, 1, 1});

  static CameraComponent CreateCamera(float fov = 1.0472f, 
                                      float nearZ = 0.1f,
                                      float farZ = 100.0f,
                                      bool primary = true);
};

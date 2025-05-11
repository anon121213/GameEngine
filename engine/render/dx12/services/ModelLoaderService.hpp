#pragma once
#include <string>
#include <vector>
#include <memory>

#include "render/components/RenderMeshComponent.hpp"

class ModelLoaderService {
public:
  // Загружает модель и возвращает один или несколько RenderMeshComponent
  static std::vector<RenderMeshComponent> LoadModel(const std::string& path);
};

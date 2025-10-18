#pragma once
#include <string>
#include <string_view>

inline std::string MakeSubmeshKey(std::string_view modelPath, std::string_view meshName) {
  std::string key;
  key.reserve(modelPath.size() + meshName.size() + 3);
  key.append(modelPath);
  key.append("::");
  key.append(meshName);
  return key;
}

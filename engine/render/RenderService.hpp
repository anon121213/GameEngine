#pragma once
#include <string>

class RenderService {
public:
    bool CreateWindow(int width, int height, const std::string& title);
    void BeginFrame();
    void EndFrame();
    void Shutdown();
};

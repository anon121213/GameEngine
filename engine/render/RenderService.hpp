#pragma once
#define WIN32_LEAN_AND_MEAN
#include <memory>
#include <windows.h>
#include <string>

#include "dx12/DX12Renderer.hpp"

class RenderService {
public:
    bool Init(HINSTANCE hInstance, int width, int height, const std::string& title);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    HWND GetHWND() const { return hwnd; }

private:
    bool CreateAppWindow(int width, int height, const std::string& title);

    HWND hwnd = nullptr;
    HINSTANCE instance = nullptr;

    std::unique_ptr<DX12Renderer> dxRenderer;
};

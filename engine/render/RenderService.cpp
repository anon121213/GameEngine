#include "RenderService.hpp"
#include <string>
#include <Windows.h>

#include "core/Log.hpp"
#include "services/ServiceLocator.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool RenderService::Init(HINSTANCE hInstance, int width, int height, const std::string& title) {
    instance = hInstance;
    if (!CreateAppWindow(width, height, title)) {
        return false;
    }

    dxRenderer = GET_SERVICE(DX12Renderer);
    if (!dxRenderer->Init(hwnd, width, height)) {
        return false;
    }

    return true;
}

void RenderService::Shutdown() {
    if (dxRenderer) {
        dxRenderer->Shutdown();
    }

    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}

bool RenderService::CreateAppWindow(int width, int height, const std::string& title) {
    const wchar_t CLASS_NAME[] = L"DX12WindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    std::wstring wTitle(title.begin(), title.end());
    hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        wTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr, nullptr,
        instance, nullptr
    );

    if (!hwnd) return false;

    ShowWindow(hwnd, SW_SHOW);
    return true;
}

void RenderService::BeginFrame() {
    if (dxRenderer) {
        dxRenderer->BeginFrame();
    }
}

void RenderService::EndFrame() {
    if (dxRenderer) {
        dxRenderer->EndFrame();
    }
}

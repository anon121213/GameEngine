#include "RenderService.hpp"
#include "core/Window.hpp"
#include <glad/glad.h>

bool RenderService::CreateWindow(int width, int height, const std::string& title) {
    return Window::Init(width, height, title);
}

void RenderService::BeginFrame() {
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderService::EndFrame() {
    Window::SwapBuffers();
}

void RenderService::Shutdown() {
    Window::Shutdown();
}

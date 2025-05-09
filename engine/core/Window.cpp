#include "Window.hpp"

#include "core/Log.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Log.hpp"

static GLFWwindow* window = nullptr;

bool Window::Init(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        LOG_ERROR("GLFW initialize fail.");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        LOG_ERROR("Create window error");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        LOG_ERROR("GLAD was not load.");
        return false;
    }

    LOG_INFO("Window crate success ({}x{})", width, height);
    return true;
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::Shutdown() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(window);
}
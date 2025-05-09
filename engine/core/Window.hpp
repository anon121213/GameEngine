#pragma once
#include <string>

class Window {
public:
    static bool Init(int width, int height, const std::string& title);
    static void PollEvents();
    static void Shutdown();
    static bool ShouldClose();
    static void SwapBuffers();
};
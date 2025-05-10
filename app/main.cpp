#include <windows.h>

#include "fsm/StateMachine.hpp"
#include "fsm/States/BootstrapState.hpp"
#include "fsm/States/StartEngineState.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout); // перенаправление std::cout

    StateMachine fsm;
    fsm.Register<BootstrapState>(std::make_unique<BootstrapState>(fsm, hInstance));
    fsm.Register<StartEngineState>(std::make_unique<StartEngineState>(fsm, hInstance));
    fsm.ChangeState<BootstrapState>();

    return 0;
}

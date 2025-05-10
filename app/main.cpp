#include <windows.h>
#include <services/ServiceLocator.hpp>

#include "fsm/StateMachine.hpp"
#include "fsm/States/BootstrapState.hpp"
#include "fsm/States/StartEngineState.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    auto fsm = std::make_shared<StateMachine>();
    ServiceLocator::Register<StateMachine>(fsm);
    fsm->Register<BootstrapState>(std::make_unique<BootstrapState>());
    fsm->Register<StartEngineState>(std::make_unique<StartEngineState>(hInstance));
    fsm->ChangeState<BootstrapState>();

    return 0;
}

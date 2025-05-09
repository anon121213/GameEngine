#include "fsm/StateMachine.hpp"
#include "fsm/States/BootstrapState.hpp"
#include "fsm/States/StartEngineState.hpp"

int main() {
    StateMachine fsm;
    fsm.Register<BootstrapState>(std::make_unique<BootstrapState>(fsm));
    fsm.Register<StartEngineState>(std::make_unique<StartEngineState>(fsm));
    fsm.ChangeState<BootstrapState>();
    return 0;
}

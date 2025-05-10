#include "fsm/states/StartEngineState.hpp"
#include "fsm/StateMachine.hpp"

#include "core/Engine.hpp"

StartEngineState::StartEngineState(HINSTANCE hinstance) {
    StartEngineState::hinstance = hinstance;
}


void StartEngineState::Enter() {
    Engine engine;
    engine.RunLoop(hinstance);
}

void StartEngineState::Exit() {
    
}
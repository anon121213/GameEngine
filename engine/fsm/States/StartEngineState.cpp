#include "fsm/states/StartEngineState.hpp"
#include "fsm/StateMachine.hpp"

#include "core/Engine.hpp"

StartEngineState::StartEngineState(StateMachine& fsm, HINSTANCE hInstance) : fsm(fsm), hInstance(hInstance) {
}

void StartEngineState::Enter() {
    Engine engine;
    engine.RunLoop(hInstance);
}

void StartEngineState::Exit() {
    
}
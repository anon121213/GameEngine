#include "fsm/states/StartEngineState.hpp"
#include "fsm/StateMachine.hpp"

#include "core/Engine.hpp"

StartEngineState::StartEngineState(StateMachine& fsm) : fsm(fsm) {}

void StartEngineState::Enter() {
    Engine engine;
    engine.RunLoop();
}

void StartEngineState::Exit() {
    
}
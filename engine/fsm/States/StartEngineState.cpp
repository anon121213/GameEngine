#include "fsm/states/StartEngineState.hpp"

#include "core/Engine.hpp"

StartEngineState::StartEngineState(const HINSTANCE hinstance) {
    StartEngineState::hinstance = hinstance;
}


void StartEngineState::Enter() {
    Engine engine;
    engine.RunEditorLoop(hinstance);
}
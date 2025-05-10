#include <windows.h>
#include <wincon.h>
#include <winbase.h>

#include "core/Log.hpp"
#include "fsm/StateMachine.hpp"
#include "BootstrapState.hpp"
#include "StartEngineState.hpp"
#include "services/ServiceLocator.hpp"

void BootstrapState::Enter() {
    LOG_INFO("BootstrapState: initializing...");

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    Log::SetLevel(Log::Level::Trace);

    auto fsm = ServiceLocator::Get<StateMachine>();
    fsm->ChangeState<StartEngineState>();
}

void BootstrapState::Exit() {
    LOG_INFO("BootstrapState: exiting.");
}
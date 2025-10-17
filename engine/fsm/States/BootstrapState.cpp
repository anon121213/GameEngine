#include <windows.h>
#include <wincon.h>
#include <winbase.h>

#include "core/Log.hpp"
#include "fsm/StateMachine.hpp"
#include "BootstrapState.hpp"
#include "StartEngineState.hpp"
#include "services/ServiceLocator.hpp"
#include "RegisterServicesState.hpp"

void BootstrapState::Enter() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    
    SetLevel(Log::Level::Trace);

    const auto fsm = ServiceLocator::Get<StateMachine>();
    fsm->ChangeState<RegisterServicesState>();
}

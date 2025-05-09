#pragma once

class IState {
public:
    virtual ~IState() = default;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
};
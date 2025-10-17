#pragma once

struct ILateUpdatable{
    virtual void OnLateUpdate(float deltaTime) = 0;
    virtual ~ILateUpdatable() = default;
};

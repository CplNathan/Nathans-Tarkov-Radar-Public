#pragma once
#include "../../Includes.h"

class TarkovMovementContext
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovMovementContext(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    Vector2 GetViewAngles1()
    {
        return GameProcess->Read<Vector2>(Address + 0x1E0);
    }

    Vector2 GetViewAngles2()
    {
        return GameProcess->Read<Vector2>(Address + 0x1E8);
    }

    Vector3 GetLocalPosition()
    {
        return GameProcess->Read<Vector3>(Address + 0x1F0);
    }
};
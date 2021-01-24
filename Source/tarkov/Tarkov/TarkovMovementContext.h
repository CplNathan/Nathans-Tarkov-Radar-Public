#pragma once
#include "Base.h"

class TarkovMovementContext : public MemoryObject
{
public:
    TarkovMovementContext(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
    }

    Vector2 GetViewAngles1()
    {
        return GameProcess->Read<Vector2>(Address + 0x200);
    }

    Vector2 GetViewAngles2()
    {
        return GameProcess->Read<Vector2>(Address + 0x208);
    }

    Vector3 GetLocalPosition()
    {
        return GameProcess->Read<Vector3>(Address + 0x210);
    }
};
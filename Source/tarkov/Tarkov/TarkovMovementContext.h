#pragma once
#include "Base.h"

struct Vector4
{
    float x, y, z, d;
};

struct Vector3
{
    float x, y, z;
};

struct Vector2
{
    float x, y;
};

struct Matrix3x4
{
    Vector4 A;
    Vector4 B;
    Vector4 C;
};

class TarkovMovementContext : public MemoryObject
{
public:
    TarkovMovementContext(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
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
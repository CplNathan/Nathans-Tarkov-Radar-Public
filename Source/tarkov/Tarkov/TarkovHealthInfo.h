#pragma once
#include "Base.h"

class TarkovHealthInfo : public MemoryObject
{
public:
    TarkovHealthInfo(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
    }

    float GetHealth()
    {
        return GameProcess->Read<float>(Address + 0x10);
    }

    float GetMaxHealth()
    {
        return GameProcess->Read<float>(Address + 0x14);
    }
};
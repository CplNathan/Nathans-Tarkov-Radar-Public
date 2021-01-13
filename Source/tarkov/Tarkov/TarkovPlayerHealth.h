#pragma once
#include "Base.h"
#include "TarkovHealthInfo.h"

class TarkovPlayerHealth : public MemoryObject
{
public:
    TarkovPlayerHealth(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
    }

    TarkovHealthInfo GetHealthInfo()
    {
        return TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18));
    }
};
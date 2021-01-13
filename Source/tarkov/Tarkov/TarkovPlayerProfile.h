#pragma once
#include "Base.h"
#include "Unity/UnityEngineString.h"
#include "TarkovPlayerInfo.h"
#include "TarkovPlayerHealth.h"

class TarkovPlayerProfile : public MemoryObject
{
public:
    TarkovPlayerProfile(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
    }

    UnityEngineString GetPlayerID()
    {
        return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
    }

    TarkovPlayerInfo GetPlayerInfo()
    {
        return TarkovPlayerInfo(GameProcess, GameProcess->Read<uint64_t>(Address + 0x28));
    }

    TarkovPlayerHealth GetPlayerHealth()
    {
        return TarkovPlayerHealth(GameProcess, GameProcess->Read<uint64_t>(Address + 0x40));
    }
};
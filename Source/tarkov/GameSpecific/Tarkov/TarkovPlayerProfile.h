#pragma once
#include "../../Includes.h"
#include "../../GameTypes/Unity/UnityEngineString.h"
#include "TarkovPlayerInfo.h"
#include "TarkovPlayerHealth.h"

class TarkovPlayerProfile
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovPlayerProfile(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
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
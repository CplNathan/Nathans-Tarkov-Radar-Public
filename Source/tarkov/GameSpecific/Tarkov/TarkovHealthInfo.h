#pragma once
#include "../../Includes.h"

class TarkovHealthInfo
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovHealthInfo(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
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
#pragma once
#include "../../Includes.h"
#include "TarkovSkeletonRoot.h"

class TarkovPlayerBody
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    TarkovPlayerBody(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    TarkovSkeletonRoot GetSkeletonRoot()
    {
        return TarkovSkeletonRoot(GameProcess, GameProcess->Read<uint64_t>(Address + 0x28));
    }
};
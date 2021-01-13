#pragma once
#include "Base.h"
#include "TarkovSkeletonRoot.h"

class TarkovPlayerBody : public MemoryObject
{
public:
    TarkovPlayerBody(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
    }

    TarkovSkeletonRoot GetSkeletonRoot()
    {
        return TarkovSkeletonRoot(GameProcess, GameProcess->Read<uint64_t>(Address + 0x28));
    }
};
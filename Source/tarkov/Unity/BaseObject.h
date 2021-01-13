#pragma once
#include "Base.h"
#include "GameObject.h"

class BaseObject : public MemoryObject
{
public:
    BaseObject(WinProcess *GameProcess, uint64_t Address)
    : MemoryObject(GameProcess, Address)
    {

    }

    GameObject GetGameObject()
    {
        return GameObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
    }

    BaseObject GetNextBaseObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x8));
    }
};
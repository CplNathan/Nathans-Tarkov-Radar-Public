#pragma once
#include "Base.h"
#include "BaseObject.h"

class GameObjectManager : public MemoryObject
{
public:
    GameObjectManager(WinProcess *GameProcess, uint64_t Address)
    : MemoryObject(GameProcess, Address)
    {

    }

    BaseObject GetLastTaggedObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address));
    }

    BaseObject GetFirstTaggedObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x8));
    }

    BaseObject GetLastActiveObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
    }

    BaseObject GetFirstActiveObject()
    {
        return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18));
    }
};
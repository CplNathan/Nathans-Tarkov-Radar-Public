#pragma once
#include "hlapi/hlapi.h"

class RelayAbstract;

class MemoryObject
{
protected:
    WinProcess *GameProcess;
    uint64_t Address;

public:
    MemoryObject()
    {
    }

    MemoryObject(WinProcess *GameProcess, uint64_t Address)
    {
        this->GameProcess = GameProcess;
        this->Address = Address;
    }

    uint64_t GetAddress()
    {
        return Address;
    }

    virtual ~MemoryObject() = default;
};

class CachedMemoryObject : public MemoryObject
{
public:
    bool bCached;

    CachedMemoryObject()
        : MemoryObject()
    {
    }

    CachedMemoryObject(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
        this->bCached = false;
    }
};

class BaseGame : public MemoryObject
{
public:
    BaseGame(WinProcess *GameProcess, uint64_t Address, RelayAbstract *Relay)
        : MemoryObject(GameProcess, Address)
    {
        this->RelayManager = Relay;
    }

protected:
    virtual void GameMain() = 0;
    RelayAbstract *RelayManager;
};
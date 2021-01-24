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

    virtual uint64_t GetAddress()
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
protected:
    WinDll* Module;

public:
    BaseGame(WinProcess *GameProcess, WinDll* Module, RelayAbstract *Relay)
        : MemoryObject(GameProcess, Module->info.baseAddress)
    {
        this->RelayManager = Relay;
        this->Module = Module;
    }

    uint64_t GetAddress() override
    {
        return Module->info.baseAddress;
    }

protected:
    virtual void GameMain() = 0;
    RelayAbstract *RelayManager;
};

struct Vector4
{
    float x, y, z, d;
};

struct Vector3
{
    float x, y, z;
};

struct Vector2
{
    float x, y;
};

struct Matrix3x4
{
    Vector4 A;
    Vector4 B;
    Vector4 C;
};

struct Matrix4x4
{
    Vector4 A;
    Vector4 B;
    Vector4 C;
    Vector4 D;
};
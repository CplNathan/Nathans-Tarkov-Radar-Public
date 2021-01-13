#pragma once
#include "Base.h"
#include "Unity/UnityEngineString.h"

class TarkovExfilPoint : public CachedMemoryObject
{
private:
    uint64_t m_eExfilSettings;

public:
    std::string ExfilName;
    std::string ID;

    TarkovExfilPoint()
        : CachedMemoryObject()
    {
    }

    TarkovExfilPoint(WinProcess *GameProcess, uint64_t Address)
        : CachedMemoryObject(GameProcess, Address)
    {
        m_eExfilSettings = GameProcess->Read<uint64_t>(Address + 0x58);

        ExfilName = GetName();
        ID = ExfilName;
        bCached = true;
    }

    int8_t GetOpenStatus()
    {
        return GameProcess->Read<int8_t>(Address + 0xA8); // 1 - Not Open, 2 - Uncomplete Requirement, 3 - Countdown, 4 - Regular/Open, 5 - Pending, 6 - AwaitingManualActivation
    }

private:
    std::string GetName()
    {
        return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(m_eExfilSettings + 0x10)).GetString();
    }

public:
    float GetExitTime()
    {
        return GameProcess->Read<float>(m_eExfilSettings + 0x24);
    }

    float GetExitMinTime()
    {
        return GameProcess->Read<float>(m_eExfilSettings + 0x30);
    }

    float GetExitMaxTime()
    {
        return GameProcess->Read<float>(m_eExfilSettings + 0x34);
    }

    float GetExitStartTime()
    {
        return GameProcess->Read<float>(m_eExfilSettings + 0x38);
    }

    float GetExfilStartTime()
    {
        return GameProcess->Read<float>(Address + 0xAC);
    }

    UnityEngineString GetHint()
    {
        return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(GameProcess->Read<uint64_t>(Address + 0x60) + 0x18));
    }

    float GetChance()
    {
        return GameProcess->Read<float>(m_eExfilSettings + 0x2C);
    }
};
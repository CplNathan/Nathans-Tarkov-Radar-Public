#pragma once
#include "Base.h"
#include "TarkovHealthInfo.h"

class TarkovPlayerHealth : public MemoryObject
{
public:
    TarkovPlayerHealth(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
    }

    // 24/01/21 - These offsets may be out of date? Either way it does not work.

    int GetMaxHealthSum()
    {
        uint64_t m_pBodyController = GameProcess->Read<uint64_t>(Address + 0x18);

        TarkovHealthInfo head = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x30));
        TarkovHealthInfo thorax = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x48));
        TarkovHealthInfo stomach = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x60));
        TarkovHealthInfo larm = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x78));
        TarkovHealthInfo rarm = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x90));
        TarkovHealthInfo lleg = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0xA8));
        TarkovHealthInfo rleg = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0xC0));
        
        return head.GetHealth() + thorax.GetHealth() + stomach.GetHealth() + larm.GetHealth() + rarm.GetHealth() + lleg.GetHealth() + rleg.GetHealth();
    }

    int GetCurrentHealthSum()
    {
        uint64_t m_pBodyController = GameProcess->Read<uint64_t>(Address + 0x18);

        TarkovHealthInfo head = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x30));
        TarkovHealthInfo thorax = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x48));
        TarkovHealthInfo stomach = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x60));
        TarkovHealthInfo larm = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x78));
        TarkovHealthInfo rarm = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0x90));
        TarkovHealthInfo lleg = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0xA8));
        TarkovHealthInfo rleg = TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(m_pBodyController + 0xC0));
        
        return head.GetMaxHealth() + thorax.GetMaxHealth() + stomach.GetMaxHealth() + larm.GetMaxHealth() + rarm.GetMaxHealth() + lleg.GetMaxHealth() + rleg.GetMaxHealth();
    }
};
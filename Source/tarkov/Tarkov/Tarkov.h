#pragma once
#include "Unity.h"
#include "TarkovPlayer.h"
#include "TarkovLootItem.h"
#include "TarkovExfilPoint.h"
#include "Helper/pattern_scan.h"

#include <list>

const struct Offsets
{
    static const uint64_t GameObjectManager = 0x156B698;
} Offsets;

class TarkovGame : public UnityGame
{
protected:
    void GameMain();

public:
    TarkovGame(WinProcess *GameProcess, WinDll* Module, RelayAbstract *Relay)
        : UnityGame(GameProcess, Module, Relay)
    {
        GameMain();
    }

    int32_t GetPlayerCount();

    std::list<TarkovPlayer> GetAllPlayers();

    std::list<TarkovLootItem> GetAllLoot();

    std::list<TarkovExfilPoint> GetExfilArray();
};
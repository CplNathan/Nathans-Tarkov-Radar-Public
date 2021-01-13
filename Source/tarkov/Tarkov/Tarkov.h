#pragma once
#include "Unity.h"
#include "TarkovPlayer.h"
#include "TarkovLootItem.h"
#include "TarkovExfilPoint.h"

#include <list>

const struct Offsets
{
    static const uint64_t GameObjectManager = 0x15181E8;
} Offsets;

class TarkovGame : public UnityGame
{
protected:
    void GameMain();

public:
    TarkovGame(WinProcess *GameProcess, uint64_t Address, RelayAbstract *Relay)
        : UnityGame(GameProcess, Address, Relay)
    {
        GameMain();
    }

    int32_t GetPlayerCount();

    std::list<TarkovPlayer> GetAllPlayers();

    std::list<TarkovLootItem> GetAllLoot();

    std::list<TarkovExfilPoint> GetExfilArray();
};
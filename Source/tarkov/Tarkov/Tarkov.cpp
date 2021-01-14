#include "Tarkov.h"
#include "TarkovPlayer.h"
#include "TarkovLootItem.h"
#include "TarkovExfilPoint.h"

#include "relayabstract.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void TarkovGame::GameMain()
{
    while (true)
    {
        sleep(1);
        GOM = GameObjectManager(GameProcess, GameProcess->Read<uint64_t>(Address + Offsets.GameObjectManager));
        LocalGameWorld = FindLocalGameWorld();

        printf("Game Object Manager: %lx\tLocal Game World: %lx\n", GOM.GetAddress(), LocalGameWorld);

        auto gameReady = [this]() {
            bool GOMValid = !(GOM.GetAddress() == 0x0);
            bool LGWValid = !(LocalGameWorld == 0x0);
            bool HasPlayers = !(GetPlayerCount() == 0);

            return GOMValid && LGWValid && HasPlayers;
        };

        RelayManager->UpdateGameReady(gameReady());

        int i = 0;
        while (gameReady())
        {
            usleep(25000);
            ++i;

            // Player Handling
            std::list<TarkovPlayer> AllPlayers = GetAllPlayers();

            // Update existing player
            for (TarkovPlayer Player : AllPlayers)
            {
                RelayManager->UpdatePlayer(Player, i);
            }

            RelayManager->DeleteOldPlayers(i);

            // Loot Handling
            std::list<TarkovLootItem> AllLoot = GetAllLoot();

            for (TarkovLootItem Loot : AllLoot)
            {
                RelayManager->UpdateLoot(Loot, i);
            }

            RelayManager->DeleteOldLoot(i);

            // Exfil Handling
            std::list<TarkovExfilPoint> AllExfils = GetExfilArray();

            for (TarkovExfilPoint Exfil : AllExfils)
            {
                RelayManager->UpdateExfil(Exfil, i);
            }
        }
    }
}

int32_t TarkovGame::GetPlayerCount()
{
    uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(LocalGameWorld + 0x70);

    return GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
}

std::list<TarkovPlayer> TarkovGame::GetAllPlayers()
{
    std::list<TarkovPlayer> PlayerList;

    uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(LocalGameWorld + 0x70);

    int32_t m_pPlayerListSize = GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
    uint64_t m_pPlayerListObject = GameProcess->Read<uint64_t>(m_pPlayerList + 0x10);

    for (int i = 0; i < m_pPlayerListSize; i++)
    {
        TarkovPlayer m_pPlayer = TarkovPlayer(GameProcess, GameProcess->Read<uint64_t>(m_pPlayerListObject + 0x20 + (i * 0x8)));
        PlayerList.push_back(m_pPlayer);
    }

    return PlayerList;
}

std::list<TarkovLootItem> TarkovGame::GetAllLoot()
{
    std::list<TarkovLootItem> LootList;

    uint64_t m_lLootList = GameProcess->Read<uint64_t>(LocalGameWorld + 0x50);

    int32_t m_lLootListSize = GameProcess->Read<int32_t>(m_lLootList + 0x18);
    uint64_t m_lLootListObject = GameProcess->Read<uint64_t>(m_lLootList + 0x10);

    for (int i = 0; i < m_lLootListSize; i++)
    {
        TarkovLootItem m_lItem = TarkovLootItem(GameProcess, GameProcess->Read<uint64_t>(m_lLootListObject + 0x20 + (i * 0x8)), i);
        Vector3 coord = m_lItem.LootLocation;
        if (coord.x == 0 && coord.y == 0 && coord.z == 0)
            continue;

        LootList.push_back(m_lItem);
    }

    return LootList;
}

std::list<TarkovExfilPoint> TarkovGame::GetExfilArray()
{
    std::list<TarkovExfilPoint> ExfilList;

    uint64_t m_eExfilController = GameProcess->Read<uint64_t>(GameProcess->Read<uint64_t>(LocalGameWorld + 0x18) + 0x20);

    int64_t m_eExfilListSize = GameProcess->Read<int64_t>(m_eExfilController + 0x18);
    for (int i = 0; i < m_eExfilListSize; i++)
    {
        TarkovExfilPoint m_eExfilPoint = TarkovExfilPoint(GameProcess, GameProcess->Read<uint64_t>(m_eExfilController + 0x20 + (i * 0x8)));
        ExfilList.push_back(m_eExfilPoint);
    }

    return ExfilList;
}
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
        if (GameProcess == nullptr || Module == nullptr)
            break;

        sleep(1);
        GOM = GameObjectManager(GameProcess, GameProcess->Read<uint64_t>(Address + Offsets.GameObjectManager));
        LocalGameWorld = FindLocalGameWorld();

        auto gameReady = [this]() {
            bool GOMValid = (GOM.GetAddress() != 0x0);
            bool LGWValid = (LocalGameWorld != 0x0);
            bool HasPlayers = (GetPlayerCount() > 0);

            return GOMValid && LGWValid && HasPlayers;
        };

        RelayManager->UpdateGameReady(gameReady());

        int i = 0;
        while (gameReady())
        {
            usleep(100000);
            ++i;

            // Player Handling
            std::vector<TarkovPlayer> AllPlayers = GetAllPlayers();

            // Update existing player
            for (const TarkovPlayer& Player : AllPlayers)
            {
                //Player.DebugDump();
                RelayManager->UpdatePlayer(Player, i);
            }

            RelayManager->DeleteOldPlayers(i);

            // Loot Handling
            std::vector<TarkovLootItem> AllLoot = GetAllLoot();

            for (const TarkovLootItem& Loot : AllLoot)
            {
                RelayManager->UpdateLoot(Loot, i);
            }

            RelayManager->DeleteOldLoot(i);

            // Exfil Handling
            std::vector<TarkovExfilPoint> AllExfils = GetExfilArray();

            for (const TarkovExfilPoint& Exfil : AllExfils)
            {
                RelayManager->UpdateExfil(Exfil, i);
            }
        }
    }
}

int32_t TarkovGame::GetPlayerCount()
{
    uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(LocalGameWorld + 0x80);

    return GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
}

std::vector<TarkovPlayer> TarkovGame::GetAllPlayers()
{
    std::vector<TarkovPlayer> PlayerList;

    uint64_t m_pPlayerList = GameProcess->Read<uint64_t>(LocalGameWorld + 0x80);

    int32_t m_pPlayerListSize = GameProcess->Read<int32_t>(m_pPlayerList + 0x18);
    std::vector<uint64_t> PlayerListPtr(m_pPlayerList, 0x0);

    uint64_t m_pPlayerListObject = GameProcess->Read<uint64_t>(m_pPlayerList + 0x10);

    GameProcess->Read(m_pPlayerListObject + 0x20, PlayerListPtr.data(), m_pPlayerListSize * sizeof(uint64_t));

    for (const uint64_t& PlayerPtr : PlayerListPtr)
    {
        PlayerList.push_back(TarkovPlayer(GameProcess, PlayerPtr));
    }

    /*
    for (int i = 0; i < m_pPlayerListSize; i++)
    {
        TarkovPlayer m_pPlayer = TarkovPlayer(GameProcess, GameProcess->Read<uint64_t>(m_pPlayerListObject + 0x20 + (i * 0x8)));
        PlayerList.push_back(m_pPlayer);
    }
    */

    return PlayerList;
}

std::vector<TarkovLootItem> TarkovGame::GetAllLoot()
{
    std::vector<TarkovLootItem> LootList;

    uint64_t m_lLootList = GameProcess->Read<uint64_t>(LocalGameWorld + 0x60);

    int32_t m_lLootListSize = GameProcess->Read<int32_t>(m_lLootList + 0x18);
    std::vector<uint64_t> LootListPtr(m_lLootListSize, 0x0);

    uint64_t m_lLootListObject = GameProcess->Read<uint64_t>(m_lLootList + 0x10);

    GameProcess->Read(m_lLootListObject + 0x20, LootListPtr.data(), m_lLootListSize * sizeof(uint64_t));

    for (const uint64_t& LootPtr : LootListPtr)
    {
        LootList.push_back(TarkovLootItem(GameProcess, LootPtr));
    }

    /*
    for (int i = 0; i < m_lLootListSize; i++)
    {
        TarkovLootItem m_lItem = TarkovLootItem(GameProcess, GameProcess->Read<uint64_t>(m_lLootListObject + 0x20 + (i * 0x8)));
        Vector3 coord = m_lItem.LootLocation;
        if (coord.x == 0 && coord.y == 0 && coord.z == 0)
            continue;

        LootList.push_back(m_lItem);
    }
    */

    return LootList;
}

std::vector<TarkovExfilPoint> TarkovGame::GetExfilArray()
{
    std::vector<TarkovExfilPoint> ExfilList;

    uint64_t m_eExfilController = GameProcess->Read<uint64_t>(GameProcess->Read<uint64_t>(LocalGameWorld + 0x18) + 0x20);

    int64_t m_eExfilListSize = GameProcess->Read<int64_t>(m_eExfilController + 0x18);
    std::vector<uint64_t> ExfilListPtr(m_eExfilListSize, 0x0);

    //uint64_t m_eExfilController = GameProcess->Read<uint64_t>(GameProcess->Read<uint64_t>(LocalGameWorld + 0x18) + 0x28); //scav

    GameProcess->Read(m_eExfilController + 0x20, ExfilListPtr.data(), m_eExfilListSize * sizeof(uint64_t));

    for (const uint64_t& ExfilPtr : ExfilListPtr)
    {
        ExfilList.push_back(TarkovExfilPoint(GameProcess, ExfilPtr));
    }

    /*
    for (int i = 0; i < m_eExfilListSize; i++)
    {
        TarkovExfilPoint m_eExfilPoint = TarkovExfilPoint(GameProcess, GameProcess->Read<uint64_t>(m_eExfilController + 0x20 + (i * 0x8)));
        ExfilList.push_back(m_eExfilPoint);
    }
    */

    return ExfilList;
}
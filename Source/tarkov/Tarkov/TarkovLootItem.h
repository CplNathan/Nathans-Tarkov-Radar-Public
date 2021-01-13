#pragma once
#include "Base.h"

class TarkovLootItem : public CachedMemoryObject
{
public:
    std::size_t ID;

    std::string LootName;
    Vector3 LootLocation;
    bool isCorpse;

    TarkovLootItem()
        : CachedMemoryObject()
    {
    }

    TarkovLootItem(WinProcess *GameProcess, uint64_t Address, int32_t ID)
        : CachedMemoryObject(GameProcess, Address)
    {
        Vector3 LootLocation = GetLootLocation();
        char id[200];
        snprintf(id, sizeof(id), "%f%f%f", LootLocation.x, LootLocation.y, LootLocation.z);
        ID = std::hash<std::string>{}(id);

        LootName = GetLootName();
        LootLocation = GetLootLocation();
        isCorpse = IsCorpse();
        bCached = true;
    }

    bool operator==(const TarkovLootItem &other)
    {
        return other.ID == ID;
    }

private:
    uint64_t GetBasicInfo()
    {
        uint64_t m_lItemProfile = GameProcess->Read<uint64_t>(Address + 0x10);

        uint64_t m_lItemBasicInfo = GameProcess->Read<uint64_t>(m_lItemProfile + 0x30);
        return m_lItemBasicInfo;
    }

    bool IsAlphabet(char c)
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '(' || c == ')' || c == '_';
    }

    std::string GetLootName()
    {
        uint64_t m_lItemBasicInfo = GetBasicInfo();

        std::string result;
        uint64_t m_lItemName = GameProcess->Read<uint64_t>(m_lItemBasicInfo + 0x60);

        for (int i = 0; i <= 100; i++)
        {
            char CurrentChar = GameProcess->Read<char>(m_lItemName + i);

            if (CurrentChar == '\0')
                break;

            if (IsAlphabet(CurrentChar))
                result.push_back(CurrentChar);
        }

        return result;
    }

    Vector3 GetLootLocation()
    {
        uint64_t m_lItemBasicInfo = GetBasicInfo();

        uint64_t m_lItemLocalization = GameProcess->Read<uint64_t>(m_lItemBasicInfo + 0x30);
        uint64_t m_lItemCoordinates = GameProcess->Read<uint64_t>(m_lItemLocalization + 0x8);
        uint64_t m_lItemLocationContainer = GameProcess->Read<uint64_t>(m_lItemCoordinates + 0x38);
        Vector3 m_lItemPosition = GameProcess->Read<Vector3>(m_lItemLocationContainer + 0xB0);

        return m_lItemPosition;
    }

    bool IsCorpse()
    {
        bool IsPlayerCorpse = GetLootName().find("Observed_PlayerSuperior") != std::string::npos;
        bool IsCorpse = GetLootName().find("LootCorpse_PlayerSuperior") != std::string::npos;
        bool IsScavCorpse = GetLootName().find("Bot") != std::string::npos;

        return IsPlayerCorpse || IsCorpse || IsScavCorpse;
    }
};
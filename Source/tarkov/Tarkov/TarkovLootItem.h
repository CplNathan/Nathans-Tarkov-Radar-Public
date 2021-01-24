#pragma once
#include "Base.h"

class TarkovLootItem : public CachedMemoryObject
{
public:
    char ID[300];

    std::string LootName;
    Vector3 LootLocation;
    bool isCorpse;

    TarkovLootItem()
        : CachedMemoryObject()
    {
    }

    TarkovLootItem(WinProcess *GameProcess, uint64_t Address)
        : CachedMemoryObject(GameProcess, Address)
    {
        LootName = GetLootName();
        LootLocation = GetLootLocation();
        isCorpse = IsCorpse();

        snprintf(ID, sizeof(ID), "%s+%i:%i:%i.", LootName.c_str(), (int)LootLocation.x, (int)LootLocation.y, (int)LootLocation.z);

        bCached = true;
    }

    bool operator==(const TarkovLootItem &other)
    {
        return other.ID == ID;
    }

    bool IsHighValue()
    {
        bool IsLedX = LootName.find("_transilluminator") != std::string::npos;
        bool IsCardA = LootName.find("key_") != std::string::npos;
        bool IsLab = LootName.find("lab") != std::string::npos;
        bool IsIntel = LootName.find("info_intelligence") != std::string::npos;
        bool IsGPU = LootName.find("video_card") != std::string::npos;
        bool IsElec = LootName.find("electr_") != std::string::npos;
        bool IsCorpseA = IsCorpse();

        return IsLedX || IsCardA || IsLab || IsCorpseA || IsIntel || IsGPU || IsElec;
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

        //std::string result;
        uint64_t m_lItemName = GameProcess->Read<uint64_t>(m_lItemBasicInfo + 0x60);

        char buff[100];
        GameProcess->Read(m_lItemName, &buff[0], sizeof(buff));

        std::string Name(buff);
        Name.erase(std::find(Name.begin(), Name.end(), '\0'), Name.end());

        /*
        for (int i = 0; i <= 100; i++)
        {
            char CurrentChar = GameProcess->Read<char>(m_lItemName + i);

            if (CurrentChar == '\0')
                break;

            if (IsAlphabet(CurrentChar))
                result.push_back(CurrentChar);
        }
        */

        return Name;
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
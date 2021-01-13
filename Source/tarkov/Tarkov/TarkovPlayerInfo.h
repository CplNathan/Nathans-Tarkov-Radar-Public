#pragma once
#include "Base.h"
#include "Unity/UnityEngineString.h"

#include <map>
#include <string>

enum EMemberCategory
{
    Default = 0,
    Developer = 1,
    UniqueId = 2,
    PressAccount = 3,
    Trader = 8,
    System = 16,
    ChatModerator = 32,
    ChatModeratorWithPermanentBan = 64,
    Emissar = 98,
    UnitTest = 128
};

class TarkovPlayerInfo : public CachedMemoryObject
{
public:
    std::map<EMemberCategory, std::string> MemberString;

    std::string PlayerName;
    std::string GroupID;
    int32_t UnixCreationTime;
    EMemberCategory AccountType;

    TarkovPlayerInfo(WinProcess *GameProcess, uint64_t Address)
        : CachedMemoryObject(GameProcess, Address)
    {
        this->GameProcess = GameProcess;
        this->Address = Address;

        MemberString[EMemberCategory::Default] = "Normal";
        MemberString[EMemberCategory::Developer] = "Developer";
        MemberString[EMemberCategory::UniqueId] = "UniqueID";
        MemberString[EMemberCategory::PressAccount] = "Press";
        MemberString[EMemberCategory::Trader] = "Trader";
        MemberString[EMemberCategory::System] = "System";
        MemberString[EMemberCategory::ChatModerator] = "Chat Mod";
        MemberString[EMemberCategory::ChatModeratorWithPermanentBan] = "Chat Mod w/Perm Ban";
        MemberString[EMemberCategory::Emissar] = "Emissar";
        MemberString[EMemberCategory::UnitTest] = "Unit Test";

        PlayerName = GetPlayerName();
        GroupID = GetGroupID();
        UnixCreationTime = GetPlayerUnixCreationTime();
        AccountType = GetPlayerAccountType();

        bCached = true;
    }

private:
    std::string GetPlayerName()
    {
        return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10)).GetString();
    }

    std::string GetGroupID()
    {
        return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18)).GetString();
    }

    int32_t GetPlayerUnixCreationTime()
    {
        return GameProcess->Read<int32_t>(Address + 0x54);
    }

    EMemberCategory GetPlayerAccountType()
    {
        return (EMemberCategory)GameProcess->Read<int32_t>(Address + 0x60);
    }
};
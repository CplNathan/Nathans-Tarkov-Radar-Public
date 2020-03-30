#pragma once
#include "../../Includes.h"
#include "../../GameTypes/Unity/UnityEngineString.h"

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

class TarkovPlayerInfo
{
public:
	WinProcess *GameProcess;
	uint64_t Address;
	std::map<EMemberCategory, std::string> MemberString;

	TarkovPlayerInfo(WinProcess *GameProc, uint64_t Addr)
	{
		GameProcess = GameProc;
		Address = Addr;

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
	}

	UnityEngineString GetPlayerName()
	{
		return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
	}

	UnityEngineString GetGroupID()
	{
		return UnityEngineString(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18));
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
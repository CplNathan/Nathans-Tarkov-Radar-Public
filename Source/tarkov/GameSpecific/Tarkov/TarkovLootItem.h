#pragma once
#include "../../Includes.h"

class TarkovLootItem
{
public:
	WinProcess *GameProcess;
	uint64_t Address;
	std::size_t Signature;

	TarkovLootItem(WinProcess *GameProc, uint64_t Addr, int32_t ID)
	{
		GameProcess = GameProc;
		Address = Addr;

		char id[200];
		snprintf(id, sizeof(id), "%f%f%f", GetLootLocation().x, GetLootLocation().y, GetLootLocation().z);
		Signature = std::hash<std::string>{}(id);
	}

	bool operator==(const TarkovLootItem& other)
    {
        return other.Signature == Signature;
    }

    uint64_t GetBasicInfo()
    {
		uint64_t m_lItemProfile = GameProcess->Read<uint64_t>(Address + 0x10);

		//uint64_t m_lItemStats = GameProcess->Read<uint64_t>(m_lItemProfile + 0x28);
		uint64_t m_lItemBasicInfo = GameProcess->Read<uint64_t>(m_lItemProfile + 0x30);
        return m_lItemBasicInfo;
    }

	void ReplaceStringInPlace(std::string& subject, const std::string& search,
		const std::string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	bool IsAlphabet(char c) {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '(' || c == ')' || c == '_';
	}

	bool IsCorpse()
	{
		bool IsPlayerCorpse = GetLootName(false).find("Observed_PlayerSuperior") != std::string::npos;
		bool IsCorpse = GetLootName(false).find("LootCorpse_PlayerSuperior") != std::string::npos;
		bool IsScavCorpse = GetLootName(false).find("Bot") != std::string::npos;
		
		return IsPlayerCorpse || IsCorpse || IsScavCorpse;
	}

	bool IsHighValue()
	{
		bool IsLedX = GetLootName(false).find("_transilluminator") != std::string::npos;
		bool IsCardA = GetLootName(false).find("key_") != std::string::npos;
		bool IsLab = GetLootName(false).find("lab") != std::string::npos;
		bool IsIntel = GetLootName(false).find("info_intelligence") != std::string::npos;
		bool IsGPU = GetLootName(false).find("video_card") != std::string::npos;
		bool IsEngine = GetLootName(false).find("barter_electr_engine") != std::string::npos;
		bool IsTetriz = GetLootName(false).find("barter_electr_tetriz") != std::string::npos;
		bool IsCorpseA = IsCorpse();
		
		return IsLedX || IsCardA || IsLab || IsCorpseA || IsIntel || IsGPU || IsEngine || IsTetriz;
	}

	bool IsBugged()
	{
		bool IsMag = GetLootName(false).find("mag_") != std::string::npos;
		
		return IsMag;
	}

	std::string GetLootName(bool Filtered)
	{
		uint64_t m_lItemBasicInfo = GetBasicInfo();

		std::string result;
		uint64_t m_lItemName = GameProcess->Read<uint64_t>(m_lItemBasicInfo + 0x60);

		for (int i = 0; i <= 100; i++)
		{
			char CurrentChar = GameProcess->Read<char>(m_lItemName + i);
			// std::tolower()

			if (CurrentChar == '\0')
				break;

			if(IsAlphabet(CurrentChar))
				result.push_back(CurrentChar);
		}

		if (Filtered)
		{
			ReplaceStringInPlace(result, "item_", "");
			ReplaceStringInPlace(result, "(Clone)", "");
			ReplaceStringInPlace(result, "Observed_PlayerSuperior", "PLAYER CORPSE");
			ReplaceStringInPlace(result, "LootCorpse_PlayerSuperior", "CORPSE");
			ReplaceStringInPlace(result, "Bot", "SCAV CORPSE");
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
};
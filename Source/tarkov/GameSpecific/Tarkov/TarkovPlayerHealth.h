#pragma once
#include "../../Includes.h"
#include "TarkovHealthInfo.h"

class TarkovPlayerHealth
{
public:
	WinProcess *GameProcess;
	uint64_t Address;

	TarkovPlayerHealth(WinProcess *GameProc, uint64_t Addr)
	{
		GameProcess = GameProc;
		Address = Addr;
	}

	TarkovHealthInfo GetHealthInfo()
	{
		return TarkovHealthInfo(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18));
	}
};
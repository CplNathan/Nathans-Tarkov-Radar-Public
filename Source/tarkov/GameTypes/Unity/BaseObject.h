#pragma once
#include "../../Includes.h"
#include "GameObject.h"

class BaseObject
{
public:
	WinProcess *GameProcess;
	uint64_t Address;

	BaseObject(WinProcess *GameProc, uint64_t Addr)
	{
		GameProcess = GameProc;
		Address = Addr;
	}

	GameObject GetGameObject()
	{
		return GameObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
	}

	BaseObject GetNextBaseObject()
	{
		return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x8));
	}

};
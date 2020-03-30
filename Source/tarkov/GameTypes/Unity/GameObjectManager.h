#pragma once
#include "../../Includes.h"
#include "BaseObject.h"

class GameObjectManager
{
public:
	WinProcess *GameProcess;
	uint64_t Address;

	GameObjectManager(WinProcess *GameProc, uint64_t Addr)
	{
		GameProcess = GameProc;
		Address = Addr;
	}

	BaseObject GetLastTaggedObject()
	{
		return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address));
	}

	BaseObject GetFirstTaggedObject()
	{
		return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x8));
	}

	BaseObject GetLastActiveObject()
	{
		return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x10));
	}

	BaseObject GetFirstActiveObject()
	{
		return BaseObject(GameProcess, GameProcess->Read<uint64_t>(Address + 0x18));
	}
};
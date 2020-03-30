#pragma once
#include "../../Includes.h"

class GameWorld
{
public:
	uint64_t Address;
	bool IsValid;

	GameWorld()
	{
		Address = 0x0;
		IsValid = false;
	}

	GameWorld(uint64_t Addr)
	{
		Address = Addr;
		IsValid = true;
	}
};
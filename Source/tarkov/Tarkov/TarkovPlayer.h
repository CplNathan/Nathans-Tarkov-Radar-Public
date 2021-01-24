#pragma once
#include "Base.h"
#include "TarkovMovementContext.h"
#include "TarkovPlayerProfile.h"
#include "TarkovPlayerBody.h"
#include "TarkovHandsController.h"

class TarkovPlayer : public MemoryObject
{
public:
    std::string ID;

    TarkovPlayer()
        : MemoryObject()
    {
    }

    TarkovPlayer(WinProcess *GameProcess, uint64_t Address)
        : MemoryObject(GameProcess, Address)
    {
        ID = GetPlayerProfile().GetPlayerID().GetString();
    }

    bool IsLocalPlayer()
    {
        return GameProcess->Read<uint64_t>(Address + 0x18) ? true : false;
    }

    bool IsScav()
    {
        int32_t Creation = GetPlayerProfile().GetPlayerInfo().UnixCreationTime;
        return Creation == 0;
    }

    bool IsPlayerScav()
    {
        return !IsScav() && GetPlayerProfile().GetPlayerInfo().PlayerName.find(" ") != std::string::npos;
    }

    bool operator==(TarkovPlayer &other)
    {
        return ID == other.ID;
    }

    TarkovMovementContext GetMovementContext()
    {
        return TarkovMovementContext(GameProcess, GameProcess->Read<uint64_t>(Address + 0x40));
    }

    TarkovPlayerBody GetPlayerBody()
    {
        return TarkovPlayerBody(GameProcess, GameProcess->Read<uint64_t>(Address + 0xA8));
    }

    TarkovPlayerProfile GetPlayerProfile()
    {
        return TarkovPlayerProfile(GameProcess, GameProcess->Read<uint64_t>(Address + 0x440));
    }

    TarkovHandsController GetPlayerHandsController()
    {
        return TarkovHandsController(GameProcess, GameProcess->Read<uint64_t>(Address + 0x488)); // 24/01/21 - These offsets may be out of date? Either way it does not work. - This used to work
    }

    TarkovPlayerHealth GetPlayerHealth()
    {
        return TarkovPlayerHealth(GameProcess, GameProcess->Read<uint64_t>(Address + 0x470)); // 24/01/21 - These offsets may be out of date? Either way it does not work. - This never worked
    }

    void DebugDump()
    {
        printf("Player Dump\n");

        TarkovPlayerProfile PlayerProfile = GetPlayerProfile();
        printf("Player Name:\t%s\n", PlayerProfile.GetPlayerInfo().PlayerName.c_str());

        printf("Player Created:\t%d\n", PlayerProfile.GetPlayerInfo().UnixCreationTime);

        printf("Player ID:\t%s\n", PlayerProfile.GetPlayerID().GetString().c_str());

        printf("Is Local?: %s\tIs Scav?:%s\n", IsLocalPlayer() ? "Yes" : "No", IsScav() ? "Yes" : "No");

        Vector3 PlayerCoordinates = GetMovementContext().GetLocalPosition();
        printf("Player Coordinates (x-y-z):\t%f-%f-%f\n", PlayerCoordinates.x, PlayerCoordinates.y, PlayerCoordinates.z);

        Vector2 ViewAngle1 = GetMovementContext().GetViewAngles1();
        printf("View Angle 1 (x-y):\t%f-%f\n", ViewAngle1.x, ViewAngle1.y);

        Vector2 ViewAngle2 = GetMovementContext().GetViewAngles2();
        printf("View Angle 2 (x-y):\t%f-%f\n", ViewAngle2.x, ViewAngle2.y);

        float health = GetPlayerHealth().GetMaxHealthSum();
        printf("Health:\t%f\n", health);
    }
};
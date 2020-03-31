#pragma once
#include "../../Includes.h"
#include "TarkovMovementContext.h"
#include "TarkovPlayerProfile.h"
#include "TarkovPlayerBody.h"
#include "TarkovHandsController.h"

class TarkovPlayer
{
public:
    WinProcess *GameProcess;
    uint64_t Address;
    std::string ID;

    TarkovPlayer(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;

        ID = GetPlayerProfile().GetPlayerID().GetString();
    }

    bool IsLocalPlayer()
    {
        return GameProcess->Read<uint64_t>(Address + 0x18) ? true : false;
    }

    bool IsScav()
    {
        int32_t Creation = GetPlayerProfile().GetPlayerInfo().GetPlayerUnixCreationTime();
        return Creation == 0;
    }

    bool IsPlayerScav()
    {
        return !IsScav() && GetPlayerProfile().GetPlayerInfo().GetPlayerName().GetString().find(" ") != std::string::npos;
    }

    bool operator==(TarkovPlayer &other)
    {
        return ID == other.ID;
    }

    TarkovMovementContext GetMovementContext()
    {
        return TarkovMovementContext(GameProcess, GameProcess->Read<uint64_t>(Address + 0x38));
    }

    TarkovPlayerBody GetPlayerBody()
    {
        return TarkovPlayerBody(GameProcess, GameProcess->Read<uint64_t>(Address + 0x88));
    }

    TarkovPlayerProfile GetPlayerProfile()
    {
        return TarkovPlayerProfile(GameProcess, GameProcess->Read<uint64_t>(Address + 0x3A8));
    }

    TarkovHandsController GetPlayerHandsController()
    {
        return TarkovHandsController(GameProcess, GameProcess->Read<uint64_t>(Address + 0x3F0));
    }

    void DebugDump()
    {
        printf("Player Dump\n");

        TarkovPlayerProfile PlayerProfile = GetPlayerProfile();
        printf("Player Name:\t%s\n", PlayerProfile.GetPlayerInfo().GetPlayerName().GetString().c_str());

        printf("Player Created:\t%d\n", PlayerProfile.GetPlayerInfo().GetPlayerUnixCreationTime());

        printf("Player ID:\t%s\n", PlayerProfile.GetPlayerID().GetString().c_str());

        printf("Is Local?: %s\tIs Scav?:%s\n", IsLocalPlayer() ? "Yes" : "No", IsScav() ? "Yes" : "No");

        TarkovHealthInfo HealthInfo = PlayerProfile.GetPlayerHealth().GetHealthInfo();
        printf("Player Health(current/max):\t%f/%f\n", HealthInfo.GetHealth(), HealthInfo.GetMaxHealth());

        Vector3 PlayerCoordinates = GetMovementContext().GetLocalPosition();
        printf("Player Coordinates (x-y-z):\t%f-%f-%f\n", PlayerCoordinates.x, PlayerCoordinates.y, PlayerCoordinates.z);

        Vector2 ViewAngle1 = GetMovementContext().GetViewAngles1();
        printf("View Angle 1 (x-y):\t%f-%f\n", ViewAngle1.x, ViewAngle1.y);

        Vector2 ViewAngle2 = GetMovementContext().GetViewAngles2();
        printf("View Angle 2 (x-y):\t%f-%f\n", ViewAngle2.x, ViewAngle2.y);
    }
};
#pragma once
#include "Base.h"
#include "Tarkov/TarkovPlayer.h"
#include "Tarkov/TarkovLootItem.h"
#include "Tarkov/TarkovExfilPoint.h"
#include "sio_client.h"
#include <map>
#include <string>

class TarkovMessageBuilder /* : public MessageBuilder */
{
public:
    static sio::object_message::ptr NewPlayerMessage(TarkovPlayer Player)
    {
        std::map<std::string, sio::message::ptr> PlayerJson;

        Vector3 Location = Player.GetPlayerBody().GetSkeletonRoot().GetCoordsFromMatrixTest();
        sio::object_message::ptr PlayerLocation = sio::object_message::create();
        std::map<std::string, sio::message::ptr> &PlayerLocationMap = PlayerLocation->get_map();
        PlayerLocationMap["x"] = sio::double_message::create(Location.x);
        PlayerLocationMap["y"] = sio::double_message::create(Location.y);
        PlayerLocationMap["z"] = sio::double_message::create(Location.z);

        Vector2 View = Player.GetMovementContext().GetViewAngles1();
        sio::object_message::ptr PlayerView = sio::object_message::create();
        std::map<std::string, sio::message::ptr> &PlayerViewMap = PlayerView->get_map();
        PlayerViewMap["x"] = sio::double_message::create(View.x);
        PlayerViewMap["y"] = sio::double_message::create(View.y);

        TarkovPlayerProfile Profile = Player.GetPlayerProfile();
        TarkovPlayerInfo Info = Profile.GetPlayerInfo();

        PlayerJson["name"] = sio::string_message::create(Player.IsScav() ? "SCAV" : Player.IsPlayerScav() ? "PLAYER SCAV" : Info.PlayerName);
        PlayerJson["id"] = sio::string_message::create(Info.PlayerName + Profile.GetPlayerID().GetString());
        PlayerJson["coordinates"] = PlayerLocation;
        PlayerJson["viewangle"] = PlayerView;
        PlayerJson["islocal"] = sio::bool_message::create(Player.IsLocalPlayer());
        PlayerJson["isscav"] = sio::bool_message::create(Player.IsScav());
        PlayerJson["isplayerscav"] = sio::bool_message::create(Player.IsPlayerScav());
        PlayerJson["groupid"] = sio::string_message::create(Info.GroupID);
        PlayerJson["membertype"] = sio::int_message::create(Info.AccountType);
        PlayerJson["weapon"] = sio::string_message::create(Player.GetPlayerHandsController().GetActiveWeaponName().GetString());

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = PlayerJson;

        return Message;
    }

    static sio::object_message::ptr UpdatePlayerMessage(TarkovPlayer Player)
    {
        std::map<std::string, sio::message::ptr> PlayerJson;

        Vector3 Location = Player.GetPlayerBody().GetSkeletonRoot().GetCoordsFromMatrixTest();
        sio::object_message::ptr PlayerLocation = sio::object_message::create();
        std::map<std::string, sio::message::ptr> &PlayerLocationMap = PlayerLocation->get_map();
        PlayerLocationMap["x"] = sio::double_message::create(Location.x);
        PlayerLocationMap["y"] = sio::double_message::create(Location.y);
        PlayerLocationMap["z"] = sio::double_message::create(Location.z);

        Vector2 View = Player.GetMovementContext().GetViewAngles1();
        sio::object_message::ptr PlayerView = sio::object_message::create();
        std::map<std::string, sio::message::ptr> &PlayerViewMap = PlayerView->get_map();
        PlayerViewMap["x"] = sio::double_message::create(View.x);
        PlayerViewMap["y"] = sio::double_message::create(View.y);

        TarkovPlayerProfile Profile = Player.GetPlayerProfile();
        TarkovPlayerInfo Info = Profile.GetPlayerInfo();

        PlayerJson["id"] = sio::string_message::create(Info.PlayerName + Profile.GetPlayerID().GetString());
        PlayerJson["coordinates"] = PlayerLocation;
        PlayerJson["viewangle"] = PlayerView;
        PlayerJson["weapon"] = sio::string_message::create(Player.GetPlayerHandsController().GetActiveWeaponName().GetString());

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = PlayerJson;

        return Message;
    }

    static sio::object_message::ptr DeletePlayerMessage(TarkovPlayer Player)
    {
        std::map<std::string, sio::message::ptr> PlayerJson;

        TarkovPlayerProfile Profile = Player.GetPlayerProfile();
        TarkovPlayerInfo Info = Profile.GetPlayerInfo();

        PlayerJson["id"] = sio::string_message::create(Info.PlayerName + Profile.GetPlayerID().GetString());

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = PlayerJson;

        return Message;
    }

    static sio::object_message::ptr AddLootMessage(TarkovLootItem Loot)
    {
        std::map<std::string, sio::message::ptr> LootJson;

        Vector3 Location = Loot.LootLocation;
        sio::object_message::ptr LootLocation = sio::object_message::create();
        std::map<std::string, sio::message::ptr> &LootLocationMap = LootLocation->get_map();
        LootLocationMap["x"] = sio::double_message::create(Location.x);
        LootLocationMap["y"] = sio::double_message::create(Location.y);
        LootLocationMap["z"] = sio::double_message::create(Location.z);

        LootJson["name"] = sio::string_message::create(Loot.LootName);
        LootJson["coordinates"] = LootLocation;
        LootJson["corpse"] = sio::bool_message::create(Loot.isCorpse);
        LootJson["signature"] = sio::int_message::create(Loot.ID);

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = LootJson;

        return Message;
    }

    static sio::object_message::ptr DeleteLootMessage(TarkovLootItem Loot)
    {
        std::map<std::string, sio::message::ptr> LootJson;

        LootJson["signature"] = sio::int_message::create(Loot.ID);

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = LootJson;

        return Message;
    }

    static sio::object_message::ptr AddExfilMessage(TarkovExfilPoint Exfil)
    {
        std::map<std::string, sio::message::ptr> ExfilJson;

        ExfilJson["id"] = sio::string_message::create(Exfil.ExfilName);
        ExfilJson["hint"] = sio::string_message::create(Exfil.GetHint().GetString());
        ExfilJson["status"] = sio::int_message::create(Exfil.GetOpenStatus());
        ExfilJson["openms"] = sio::int_message::create(Exfil.GetExitStartTime());
        ExfilJson["chance"] = sio::int_message::create(Exfil.GetChance());

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = ExfilJson;

        return Message;
    }

    static sio::object_message::ptr UpdateExfilMessage(TarkovExfilPoint Exfil)
    {
        std::map<std::string, sio::message::ptr> ExfilJson;

        ExfilJson["id"] = sio::string_message::create(Exfil.ExfilName);
        ExfilJson["status"] = sio::int_message::create(Exfil.GetOpenStatus());

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = ExfilJson;

        return Message;
    }

    static sio::object_message::ptr AuthenticateMessage(std::string Passcode)
    {
        std::map<std::string, sio::message::ptr> RegisterJson;

        RegisterJson["Passcode"] = sio::string_message::create(Passcode);
        RegisterJson["isHost"] = sio::bool_message::create(true);
        RegisterJson["isClient"] = sio::bool_message::create(false);

        sio::object_message::ptr Message = sio::object_message::create();
        Message->get_map() = RegisterJson;

        return Message;
    }
};
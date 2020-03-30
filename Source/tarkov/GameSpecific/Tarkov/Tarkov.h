#pragma once
#include "../Unity.h"
#include "TarkovPlayer.h"
#include "TarkovLootItem.h"
#include "TarkovExfilPoint.h"
#include "../../Sockets/wsclient.hpp"

const struct Offsets
{
	static const uint64_t GameObjectManager = 0x15181E8;
} Offsets;

class TarkovGame : public UnityGame
{
protected:
    void EntryPointImpl() override {
		using easywsclient::WebSocket;
		WebSocket::pointer ws = WebSocket::from_url("ws://example.com"); // Had issues with wss and SSL handshake.

		if (ws == nullptr)
		{
			fprintf(dfile, "Websocket connection failure, exiting.\n");
			return;
		}

		ws->send("{\"event\":\"host\"}");
		ws->poll();

		while (true)
		{
			ws->send("{\"event\":\"keepalive\"}");
			ws->poll();

			GOM = GameObjectManager(Process, Process->Read<uint64_t>(ModuleBase + Offsets.GameObjectManager));
			LocalGameWorld = FindLocalGameWorld();

			fprintf(dfile, "Game Object Manager: %lx\tLocal Game World: %lx\n", GOM.Address, LocalGameWorld.Address);

			std::list<TarkovPlayer> TransmittedPlayers;
			std::list<TarkovLootItem> TransmittedLoot;
			std::list<TarkovExfilPoint> TransmittedExfils;

			if (ws->getReadyState() != 3)
			{
				fprintf(dfile, "Websocket connection failure, exiting.\n");
				break;
			}

			if (GOM.Address == 0x0)
			{
				fprintf(dfile, "GOM invalid, exiting.\n");
				break;
			}

			if (!LocalGameWorld.IsValid || GetPlayerCount() == 0)
			{
				printf("Game is not active, sleeping 1s\n");

				sleep(1);
				continue;
			}

			ws->send("{\"event\":\"gamestart\"}");
			printf("Game Started, socket ready and broadcasting.\n");
			while (true)
			{
				/* Keep Alive */
				ws->send("{\"event\":\"keepalive\"}");
				ws->poll();

				if (ws->getReadyState() != 3) break;
				if(GetPlayerCount() == 0)
				{
					ws->send("{\"event\":\"gameend\"}");
					printf("Game Ended\n");
					break;
				}

				usleep(25000);

				{
					std::list<TarkovPlayer> AllPlayers = GetAllPlayers();
					std::list<TarkovPlayer> NewPlayers = FindNewPlayers(AllPlayers, TransmittedPlayers);

					/* Send new player */
					for (TarkovPlayer Player : NewPlayers) {
						TarkovPlayerProfile Profile = Player.GetPlayerProfile();
						TarkovPlayerInfo Info = Profile.GetPlayerInfo();

						Vector3 Location = Player.GetPlayerBody().GetSkeletonRoot().GetCoordsFromMatrixTest();

						Vector2 View = Player.GetMovementContext().GetViewAngles1();
						/* send */
						char buff[512];
						snprintf(buff, sizeof(buff),"{\"event\":\"newplayer\",\"message\":{\"name\":\"%s\",\"id\":\"%s\",\"coordinates\":[%f,%f,%f],\"viewangle\":[%f,%f],\"islocal\":%s,\"isscav\":%s,\"isplayerscav\":%s,\"groupid\":\"%s\",\"membertype\":\"%s\",\"weapon\":\"%s\"}}",
							Player.IsScav() ? "SCAV" : Player.IsPlayerScav() ? "PLAYER SCAV" : escape_json(Info.GetPlayerName().GetString()).c_str(),
							escape_json(Info.GetPlayerName().GetString() + Profile.GetPlayerID().GetString()).c_str(),
							Location.x, Location.y, Location.z,
							View.x, View.y,
							Player.IsLocalPlayer() ? "true" : "false",
							Player.IsScav() ? "true" : "false",
							Player.IsPlayerScav() ? "true" : "false",
							escape_json(Info.GetGroupID().GetString()).c_str(),
							Info.MemberString[Info.GetPlayerAccountType()].c_str(),
							escape_json(Player.GetPlayerHandsController().GetActiveWeaponName().GetString()).c_str()
						);
						ws->send(buff);
						TransmittedPlayers.push_back(Player);
					}

					/* Update existing player */
					for (TarkovPlayer Player : AllPlayers) {
						TarkovPlayerProfile Profile = Player.GetPlayerProfile();

						Vector3 Location = Player.GetPlayerBody().GetSkeletonRoot().GetCoordsFromMatrixTest();

						Vector2 View = Player.GetMovementContext().GetViewAngles1();
						/* send */
						char buff[512];
						snprintf(buff, sizeof(buff), "{\"event\":\"updateplayer\",\"message\":{\"id\":\"%s\",\"coordinates\":[%f,%f,%f],\"viewangle\":[%f,%f],\"weapon\":\"%s\"}}",
							escape_json(Profile.GetPlayerInfo().GetPlayerName().GetString() + Profile.GetPlayerID().GetString()).c_str(),
							Location.x, Location.y, Location.z,
							View.x, View.y,
							escape_json(Player.GetPlayerHandsController().GetActiveWeaponName().GetString()).c_str()
						);
						ws->send(buff);
					}

					std::list<TarkovPlayer> DeletedPlayers = FindNewPlayers(TransmittedPlayers, AllPlayers);

					/* Update existing player */
					for (TarkovPlayer Player : DeletedPlayers) {
						TarkovPlayerProfile Profile = Player.GetPlayerProfile();
						/* send */
						char buff[512];
						snprintf(buff, sizeof(buff), "{\"event\":\"deleteplayer\",\"message\":{\"id\":\"%s\"}}",
							escape_json(Profile.GetPlayerInfo().GetPlayerName().GetString() + Profile.GetPlayerID().GetString()).c_str()
						);
						ws->send(buff);
					}
				}

				{
					std::list<TarkovLootItem> AllLoot = GetAllLoot();
					std::list<TarkovLootItem> NewLoot = FindNewLoot(AllLoot, TransmittedLoot);

					//ws->send("{\"event\": \"clearloot\"}");
					for (TarkovLootItem Loot : NewLoot) {
						/* send */
						char buff[256];
						snprintf(buff, sizeof(buff), "{\"event\":\"addloot\",\"message\":{\"name\":\"%s\",\"coordinates\":[%f,%f,%f],\"highvalue\":%s,\"corpse\":%s,\"signature\":\"%ldd\"}}",
							Loot.GetLootName(true).c_str(),
							Loot.GetLootLocation().x, Loot.GetLootLocation().y, Loot.GetLootLocation().z,
							Loot.IsHighValue() ? "true" : "false",
							Loot.IsCorpse() ? "true" : "false",
							Loot.Signature
						);
						ws->send(buff);
						TransmittedLoot.push_back(Loot);
					}

					std::list<TarkovLootItem> DeletedLoot = FindNewLoot(TransmittedLoot, AllLoot);

					for (TarkovLootItem Loot : DeletedLoot) {
						/* send */
						char buff[256];
						snprintf(buff, sizeof(buff), "{\"event\":\"deleteloot\",\"message\":{\"signature\":\"%ldd\"}}",
							Loot.Signature
						);
						ws->send(buff);
						TransmittedLoot.remove(Loot);
					}
				}

				{
					std::list<TarkovExfilPoint> AllExfils = GetExfilArray();
					std::list<TarkovExfilPoint> NewExfils = FindNewExfils(AllExfils, TransmittedExfils);

					for (TarkovExfilPoint Exfil : NewExfils) {
						char buff[512];
						snprintf(buff, sizeof(buff), "{\"event\":\"addexfil\",\"message\":{\"id\":\"%s\",\"hint\":\"%s\",\"status\":\"%" PRId8 "\",\"openms\":\"%f\",\"chance\":\"%f\"}}",
							escape_json(Exfil.GetName().GetString()).c_str(),
							escape_json(Exfil.GetHint().GetString()).c_str(),
							Exfil.GetOpenStatus(),
							Exfil.GetExitStartTime(),
							Exfil.GetChance()
						);
						ws->send(buff);
						TransmittedExfils.push_back(Exfil);
					}

					for (TarkovExfilPoint Exfil : AllExfils) {
						char buff[512];
						snprintf(buff, sizeof(buff), "{\"event\":\"updateexfil\",\"message\":{\"id\":\"%s\",\"status\":\"%" PRId8 "\"}}",
							escape_json(Exfil.GetName().GetString()).c_str(),
							Exfil.GetOpenStatus()
						);
						ws->send(buff);
					}
				}
			}
		}

		delete ws;
    };

public:
	int32_t GetPlayerCount()
	{
		uint64_t m_pPlayerList = Process->Read<uint64_t>(LocalGameWorld.Address + 0x70);

		return Process->Read<int32_t>(m_pPlayerList + 0x18);
	}

	std::list<TarkovPlayer> GetAllPlayers()
	{
		std::list<TarkovPlayer> PlayerList;

		uint64_t m_pPlayerList = Process->Read<uint64_t>(LocalGameWorld.Address + 0x70);

		int32_t m_pPlayerListSize = Process->Read<int32_t>(m_pPlayerList + 0x18);
		uint64_t m_pPlayerListObject = Process->Read<uint64_t>(m_pPlayerList + 0x10);

		for (int i = 0; i < m_pPlayerListSize; i++)
		{
			TarkovPlayer m_pPlayer = TarkovPlayer(Process, Process->Read<uint64_t>(m_pPlayerListObject + 0x20 + (i * 0x8)));
			PlayerList.push_back(m_pPlayer);
		}
		
		return PlayerList;
	}

	std::list<TarkovPlayer> FindNewPlayers(std::list<TarkovPlayer> Players, std::list<TarkovPlayer> tPlayers)
	{
		std::list<TarkovPlayer> Temp;

		for (TarkovPlayer& Player : Players) {
			bool Found = false;
			for (TarkovPlayer& tPlayer : tPlayers)
			{
				if (Player.ID == tPlayer.ID)
				{
					Found = true;
					break;
				}
			}
			if (!Found)
				Temp.push_back(Player);
		}

		return Temp;
	}

	std::list<TarkovLootItem> GetAllLoot()
	{
		std::list<TarkovLootItem> LootList;

		uint64_t m_lLootList = Process->Read<uint64_t>(LocalGameWorld.Address + 0x50);

		int32_t m_lLootListSize = Process->Read<int32_t>(m_lLootList + 0x18);
		uint64_t m_lLootListObject = Process->Read<uint64_t>(m_lLootList + 0x10);

		for (int i = 0; i < m_lLootListSize; i++)
		{
			TarkovLootItem m_lItem = TarkovLootItem(Process, Process->Read<uint64_t>(m_lLootListObject + 0x20 + (i * 0x8)), i);
			Vector3 coord = m_lItem.GetLootLocation();
			if (coord.x == 0 && coord.y == 0 && coord.z == 0)
				continue;

			if (m_lItem.IsBugged())
				continue;

			LootList.push_back(m_lItem);
		}

		return LootList;
	}

	std::list<TarkovLootItem> FindNewLoot(std::list<TarkovLootItem> Loot, std::list<TarkovLootItem> tLoot)
	{
		std::list<TarkovLootItem> Temp;

		for (TarkovLootItem& Item : Loot) {
			bool Found = false;
			for (TarkovLootItem& tItem : tLoot)
			{
				if (Item.Signature == tItem.Signature)
				{
					Found = true;
					break;
				}
			}
			if (!Found)
				Temp.push_back(Item);
		}

		return Temp;
	}

	std::list<TarkovExfilPoint> GetExfilArray()
	{
		std::list<TarkovExfilPoint> ExfilList;

		uint64_t m_eExfilController = Process->Read<uint64_t>(Process->Read<uint64_t>(LocalGameWorld.Address + 0x18) + 0x20); 
		//uint64_t m_eExfilController = Process->Read<uint64_t>(Process->Read<uint64_t>(LocalGameWorld.Address + 0x20) + 0x20); // scav

		int64_t m_eExfilListSize = Process->Read<int64_t>(m_eExfilController + 0x18);
		for (int i = 0; i < m_eExfilListSize; i++)
		{
			TarkovExfilPoint m_eExfilPoint = TarkovExfilPoint(Process, Process->Read<uint64_t>(m_eExfilController + 0x20 + (i * 0x8)));
			ExfilList.push_back(m_eExfilPoint);
		}

		return ExfilList;
	}

	std::list<TarkovExfilPoint> FindNewExfils(std::list<TarkovExfilPoint> Exfils, std::list<TarkovExfilPoint> tExfils)
	{
		std::list<TarkovExfilPoint> Temp;

		for (TarkovExfilPoint& Exfil : Exfils) {
			bool Found = false;
			for (TarkovExfilPoint& tExfil : tExfils)
			{
				if (Exfil.GetName().GetString() == tExfil.GetName().GetString())
				{
					Found = true;
					break;
				}
			}
			if (!Found)
				Temp.push_back(Exfil);
		}

		return Temp;
	}
};
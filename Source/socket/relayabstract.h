#pragma once
#include "sio_client.h"
#include "sio_message.h"
#include "tarkovmessages.h"
#include "connectionlistener.h"
#include <vector>
#include <list>
#include <algorithm>

template <class T>
struct TrackedMember
{
public:
    int i;
    T Member;

    TrackedMember(T Member)
    {
        i = 0;
        this->Member = Member;
    }

    bool operator==(const TrackedMember &other)
    {
        return other.Member.ID == Member.ID;
    }
};

class RelayAbstract
{
private:
    sio::socket::ptr Socket;
    connection_listener *Listener;

    std::string VMStatus = " Unknown";
    bool gameReady = false;

    std::vector<TarkovPlayer> TransmittedPlayers;

public:
    RelayAbstract(sio::socket::ptr Socket, connection_listener *Listener)
    {
        this->Socket = Socket;
        this->Listener = Listener;
    }

    std::string ReadVMStatus()
    {
        return this->VMStatus;
    }

    void UpdateVMStatus(std::string message)
    {
        this->VMStatus = message;
    }

    bool ReadGameReady()
    {
        return gameReady;
    }

    void UpdateGameReady(bool ready)
    {
        if (this->gameReady != ready)
            UpdateGame(ready);

        this->gameReady = ready;
    }

private:
    std::vector<TrackedMember<TarkovPlayer>> SentPlayers;
    std::vector<TrackedMember<TarkovLootItem>> SentLoot;
    std::vector<TrackedMember<TarkovExfilPoint>> SentExfils;

public:
    void UpdatePlayer(TarkovPlayer Player, int i)
    {
        if (!Listener->connect_active)
            return;

        auto it = std::find(SentPlayers.begin(), SentPlayers.end(), Player);
        if (it != SentPlayers.end())
        {
            int index = it - SentPlayers.begin();
            Socket->emit("updateplayer", TarkovMessageBuilder::UpdatePlayerMessage(Player));
            SentPlayers[index].i = i;
        }
        else
        {
            Socket->emit("newplayer", TarkovMessageBuilder::NewPlayerMessage(Player));
            TrackedMember<TarkovPlayer> NewMember(Player);
            NewMember.i = i;
            SentPlayers.push_back(NewMember);
        }
    }

    void DeleteOldPlayers(int i)
    {
        if (!Listener->connect_active)
            return;

        std::list<TrackedMember<TarkovPlayer>> OldPlayers;
        for (TrackedMember<TarkovPlayer> Player : SentPlayers)
        {
            if (Player.i < i)
                OldPlayers.push_back(Player);
        }

        for (TrackedMember<TarkovPlayer> Player : OldPlayers)
        {
            auto it = std::find(SentPlayers.begin(), SentPlayers.end(), Player.Member);
            Socket->emit("deleteplayer", TarkovMessageBuilder::DeletePlayerMessage(Player.Member));

            if (it != SentPlayers.end())
                SentPlayers.erase(it);
        }
    }

    void UpdateLoot(TarkovLootItem Loot, int i)
    {
        if (!Listener->connect_active)
            return;

        auto it = std::find(SentLoot.begin(), SentLoot.end(), Loot);
        if (it != SentLoot.end())
        {
            int index = it - SentLoot.begin();
            SentLoot[index].i = i;
        }
        else
        {
            Socket->emit("addloot", TarkovMessageBuilder::AddLootMessage(Loot));
            TrackedMember<TarkovLootItem> NewLoot(Loot);
            NewLoot.i = i;
            SentLoot.push_back(NewLoot);
        }
    }

    void DeleteOldLoot(int i)
    {
        if (!Listener->connect_active)
            return;

        std::list<TrackedMember<TarkovLootItem>> OldLoot;
        for (TrackedMember<TarkovLootItem> Loot : SentLoot)
        {
            if (Loot.i < i)
                OldLoot.push_back(Loot);
        }

        for (TrackedMember<TarkovLootItem> Loot : OldLoot)
        {
            auto it = std::find(SentLoot.begin(), SentLoot.end(), Loot.Member);
            Socket->emit("deleteloot", TarkovMessageBuilder::DeleteLootMessage(Loot.Member));

            if (it != SentLoot.end())
                SentLoot.erase(it);
        }
    }

    void UpdateExfil(TarkovExfilPoint Exfil, int i)
    {
        if (!Listener->connect_active)
            return;

        auto it = std::find(SentExfils.begin(), SentExfils.end(), Exfil);
        if (it != SentExfils.end())
        {
            int index = it - SentExfils.begin();
            Socket->emit("updateexfil", TarkovMessageBuilder::UpdateExfilMessage(Exfil));
            SentExfils[index].i = i;
        }
        else
        {
            Socket->emit("addexfil", TarkovMessageBuilder::AddExfilMessage(Exfil));
            TrackedMember<TarkovExfilPoint> NewExfil(Exfil);
            NewExfil.i = i;
            SentExfils.push_back(NewExfil);
        }
    }

    void UpdateGame(bool NewState)
    {
        if (!Listener->connect_active)
            return;

        if (NewState)
        {
            Socket->emit("gamestart");
        }
        else
        {
            Socket->emit("gameend");
            SentExfils.empty();
            SentLoot.empty();
            SentPlayers.empty();
        }
    }

    void Authenticate(std::string Passcode)
    {
        Socket->emit("authenticate", TarkovMessageBuilder::AuthenticateMessage(Passcode), [this](sio::message::list const &msg) {
            Listener->on_authenticated(msg[0]->get_map()["success"]->get_bool());
        });
    }
};
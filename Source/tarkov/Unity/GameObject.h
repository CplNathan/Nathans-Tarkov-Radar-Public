#pragma once
#include "Base.h"

class GameObject : public MemoryObject
{
public:
    GameObject(WinProcess *GameProcess, uint64_t Address)
    : MemoryObject(GameProcess, Address)
    {

    }

    std::string GetGameObjectName()
    {
        //std::string result;
        uint64_t GameObjectNameAddr = GameProcess->Read<uint64_t>(Address + 0x60);

        //for (int i = 0; i <= 100; i++)
        //{
            //char CurrentChar = GameProcess->Read<char>(GameObjectNameAddr + i);

            //if (CurrentChar == '\0')
                //break;

            //result.push_back(CurrentChar);
        //}

        char buff[100];
        GameProcess->Read(GameObjectNameAddr, &buff[0], sizeof(buff));

        std::string Name(buff);
        Name.erase(std::find(Name.begin(), Name.end(), '\0'), Name.end());

        return Name;
    }

    uint64_t GetGameComponent()
    {
        uint64_t ObjectClass = GameProcess->Read<uint64_t>(Address + 0x30);
        uint64_t Entity = GameProcess->Read<uint64_t>(ObjectClass + 0x18);
        uint64_t BaseEntity = GameProcess->Read<uint64_t>(Entity + 0x28);
        return BaseEntity;
    }
};
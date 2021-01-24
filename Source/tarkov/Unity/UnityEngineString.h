#pragma once
#include "Base.h"

class UnityEngineString : public MemoryObject
{
private:
    int32_t GetStringSize()
    {
        return GameProcess->Read<int32_t>(Address + 0x10);
    }

public:
    UnityEngineString(WinProcess *GameProcess, uint64_t Address)
    : MemoryObject(GameProcess, Address)
    {
        
    }

    std::string GetString()
    {
        int32_t StringSize = GetStringSize();
        std::string result(StringSize, ' ');

        GameProcess->Read(Address + 0x14, result.data(), StringSize * sizeof(short));

        /*
        for (int i = 0; i < StringSize; i++)
        {
            char CurrentChar = GameProcess->Read<char>(Address + 0x14 + (i * 2));
            result.push_back(CurrentChar);
        }
        */

        return result;
    }
};
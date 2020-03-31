#pragma once
#include "../../Includes.h"

class UnityEngineString
{
public:
    WinProcess *GameProcess;
    uint64_t Address;

    UnityEngineString(WinProcess *GameProc, uint64_t Addr)
    {
        GameProcess = GameProc;
        Address = Addr;
    }

    int32_t GetStringSize()
    {
        return GameProcess->Read<int32_t>(Address + 0x10);
    }

    std::string GetString()
    {
        std::string result;

        int32_t StringSize = GetStringSize();

        for (int i = 0; i < StringSize; i++)
        {
            char CurrentChar = GameProcess->Read<char>(Address + 0x14 + (i * 2));
            result.push_back(CurrentChar);
        }

        return result;
    }

    std::string GetStringSingle()
    {
        std::string result;

        int32_t StringSize = GetStringSize();

        for (int i = 0; i < StringSize; i++)
        {
            char CurrentChar = GameProcess->Read<char>(Address + 0x14 + (i));
            result.push_back(CurrentChar);
        }

        return result;
    }
};
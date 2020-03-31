#pragma once
#include "../Includes.h"

class BaseGame
{
protected:
    WinProcess *Process;
    uint64_t ModuleBase;

public:
    void EntryPointModule(WinProcess *InProcess, uint64_t InModuleBase)
    {
        Process = InProcess;
        ModuleBase = InModuleBase;

        printf("Running Implementation\n");
        EntryPointImpl();
    };

    virtual ~BaseGame() = default;

protected:
    virtual void EntryPointImpl(){};
};
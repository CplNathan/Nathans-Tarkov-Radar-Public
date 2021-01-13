#pragma once
#include "Base.h"
#include "Unity/GameObjectManager.h"
#include "Unity/UnityEngineString.h"

class UnityGame : public BaseGame
{
public:
    UnityGame(WinProcess *GameProcess, uint64_t Address, RelayAbstract *Relay)
        : BaseGame(GameProcess, Address, Relay)
    {
    }

protected:
    GameObjectManager GOM{0x0, 0x0};
    uint64_t LocalGameWorld{0x0};

    uint64_t FindLocalGameWorld()
    {
        int32_t CurrentIt = 0;
        for (BaseObject ActiveObject = GOM.GetFirstActiveObject(); ActiveObject.GetAddress() != GOM.GetLastActiveObject().GetAddress(); ActiveObject = ActiveObject.GetNextBaseObject())
        {
            if (++CurrentIt > 50000)
            {
                printf("GameWorld not found.");
                break;
            }
            std::string ActiveObjectName = ActiveObject.GetGameObject().GetGameObjectName();
            if (ActiveObjectName.find("GameWorld") != std::string::npos)
            {
                return ActiveObject.GetGameObject().GetGameComponent();
            }
        }

        return 0x0;
    }

    bool GameWorldActive()
    {
        return FindLocalGameWorld() != 0x0;
    }
};
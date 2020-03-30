#pragma once
#include "Base.h"
#include "../GameTypes/Unity/GameObjectManager.h"
#include "../GameTypes/Unity/UnityEngineString.h"

class UnityGame : public BaseGame
{
protected:
    GameObjectManager GOM{ 0x0, 0x0 };
    GameWorld LocalGameWorld{ 0x0 };

    GameWorld FindLocalGameWorld()
    {
        int32_t LoopDetector = 0;
        for (BaseObject ActiveObject = GOM.GetFirstActiveObject(); ActiveObject.Address != GOM.GetLastActiveObject().Address; ActiveObject = ActiveObject.GetNextBaseObject())
        {
            if (LoopDetector > 50000)
            {
                printf("GameWorld not found.");
                break;
            }
            std::string ActiveObjectName = ActiveObject.GetGameObject().GetGameObjectName();
            if (ActiveObjectName.find("GameWorld") != std::string::npos)
            {
                return ActiveObject.GetGameObject().GetLocalGameWorld();
            }
            LoopDetector++;
        }

        return GameWorld();
    }

    bool GameWorldActive()
    {
        return FindLocalGameWorld().IsValid;
    }
};
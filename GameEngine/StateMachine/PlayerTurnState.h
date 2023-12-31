#pragma once
#include "GameState.h"

#include "../Global.h"
#include "../SubSystems/SubsystemCollection.h"

class PlayerTurnState : public GameState
{
public:
    PlayerTurnState();
    void DoState() override;
    void Begin() override;
    GameState* Finish(class GameState* currentState) override;
    void ProcessInput() override;

    void SetMasterState(GameState* masterGameState) override;
    bool Enter() override;
    void SwapPokemon() override;

private:
    GameState* mMasterState;
   

  
};

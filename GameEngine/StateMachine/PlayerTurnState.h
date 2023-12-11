#pragma once
#include "game_state.h"

class player_turn_state : public game_state
{
public:

    player_turn_state();
    void DoState() override;
    void Begin() override;
    game_state* Finish(class game_state* currentState) override;
    void ProcessInput() override;

    void SetMasterState(game_state* master_game_state) override;
private:
    game_state* master_state;

  
};
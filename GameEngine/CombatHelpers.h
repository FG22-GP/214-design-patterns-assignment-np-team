#pragma once
#include "MoveData.h"
#include "RawPokemonData.h"

class CombatHelpers
{
public:
    static unsigned int CalculateAbilityDamage(RawPokemonData Attacker, RawPokemonData Defender,MoveData MoveData);
};

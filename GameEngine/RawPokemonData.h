#pragma once

struct RawPokemonData
{
    unsigned int Species : 8;

   /* unsigned char NickName_0 : 8;
    unsigned char NickName_1 : 8;
    unsigned char NickName_2 : 8;
    unsigned char NickName_3 : 8;
    unsigned char NickName_4 : 8;
    unsigned char NickName_5 : 8;
    unsigned char NickName_6 : 8;
    unsigned char NickName_7 : 8;
    unsigned char NickName_8 : 8;
    unsigned char NickName_9 : 8; 
    */

    const char* NickName;
    
    unsigned int Health : 8;
    unsigned int BaseHealth : 8;
    unsigned int BaseSpeed : 8;
    unsigned int BaseDefence : 8;
    unsigned int BaseSpecialDefence : 8;
    unsigned int BaseAttack : 8;
    unsigned int BaseSpecialAttack : 8;

    /*unsigned int HealthEv : 16;
    unsigned int SpeedEv : 16;
    unsigned int DefenceEv : 16;
    unsigned int SpecialDefenceEv : 16;
    unsigned int AttackEv : 16;
    unsigned int SpecialAttackEv : 16;
    */
    
    unsigned int Status : 3;
    unsigned int Level : 7;


    void SetName(const char* string);
};

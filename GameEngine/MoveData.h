#pragma once

#define MOVETYPE_PHYSICAL =  0x01 //0b00000001;
#define MOVETYPE_SPECIAL =  0x02 //0b00000010;
#define MOVETYPE_STATUS =  0x03 //0b00000011;

#define SPECIAL_TYPE_NONE = 0x00;
#define SPECIAL_TYPE_NORMAL = 0x01 //0b00000001;
#define SPECIAL_TYPE_FIRE = 0x02 //0b00000010;
#define SPECIAL_TYPE_WATER = 0x03 //0b00000011;
#define SPECIAL_TYPE_GRASS = 0x04 //0b00000100;
#define SPECIAL_TYPE_ELECTRIC = 0x05 //0b00000101;
#define SPECIAL_TYPE_ICE = 0x06 //0b00000110;
#define SPECIAL_TYPE_FIGHTING = 0x07 //0b00000111;
#define SPECIAL_TYPE_POISON = 0x08 //0b00001000;
#define SPECIAL_TYPE_GROUND = 0x09 //0b00001001;
#define SPECIAL_TYPE_FLYING = 0x0a //0b00001010;
#define SPECIAL_TYPE_PSYCHIC = 0x0b //0b0001011;
#define SPECIAL_TYPE_BUG = 0x0c //0b00001100;
#define SPECIAL_TYPE_ROCK = 0x0d //0b00001101;
#define SPECIAL_TYPE_GHOST = 0x0e //0b00001110;
#define SPECIAL_TYPE_DRAGON = 0x0f //0b00001111;
#define SPECIAL_TYPE_DARK = 0x10 //0b00010000;
#define SPECIAL_TYPE_STEEL = 0x11 //0b00010001;

struct MoveData
{
public:
    MoveData();
    MoveData(unsigned int MoveType,unsigned int SpecialType_1,unsigned int SpecialType_2,unsigned int Damage);
    unsigned int MoveType : 2;
    unsigned int SpecialType_1:5;
    unsigned int SpecialType_2:5;
    unsigned int Damage: 7;
};

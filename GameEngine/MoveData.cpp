#include "MoveData.h"

MoveData::MoveData(unsigned MoveType,unsigned int SpecialType_1,unsigned int SpecialType_2,unsigned int Damage)
{
    this->MoveType = MoveType;
    this->SpecialType_1 = SpecialType_1;
    this->SpecialType_2 = SpecialType_2;
    this->Damage = Damage;
}

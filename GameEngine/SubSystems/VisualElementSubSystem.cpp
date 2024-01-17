#include "VisualElementSubSystem.h"
#include "SubsystemCollection.h"
#include "../Global.h"

VisualElementSubSystem::VisualElementSubSystem()
{
    factory = new VisualElementFactory();
    isDirty = false;
}

VisualElementSubSystem::~VisualElementSubSystem()
{
    delete factory;
}

void VisualElementSubSystem::SetBackground(int spriteIndex)
{
    RectTransform rect_transform;
    
    rect_transform.position.x = INTERNAL_SCREEN_WIDTH/2;
    rect_transform.originAnchorPoint = top;
    //Potential Memory issues?? 
    backgroundVisualElement = factory->CreateVisualElement("Resources/Sprites/Backgrounds/Backgrounds.png",rect_transform,spriteIndex,2,1);

    //Make factory create visual element set the dirty bool, it's better
    isDirty = true;
}

void VisualElementSubSystem::CreateBattleSprites()
{
    VisualElementSubSystem* sub_system = SUBSYSTEM_COLLECTION->GetSubSystem<VisualElementSubSystem>();

    RectTransform player_sprite_rect{};
    
    playerPokemonVisualElement = sub_system->CreateVisualElement("Resources/Sprites/Characters/Martial Hero/Martial Hero/Sprites/Idle.png",player_sprite_rect,0,8,1);
    playerPokemonVisualElement->GetTransform()->originAnchorPoint = center;
    playerPokemonVisualElement->GetTransform()->position.y = (INTERNAL_SCREEN_HEIGHT/2);
    playerPokemonVisualElement->GetTransform()->position.x = 60;
    

    //printf("Position of pokemon sprite y: %d",PlayerPokemon->rectTransform.Position.y);
    RectTransform enemy_sprite_rect{};
    enemyPokemonVisualElement = sub_system->CreateVisualElement("Resources/Sprites/Characters/Martial Hero 2/Martial Hero 2/Sprites/Idle.png",enemy_sprite_rect,1,4,1);
    enemyPokemonVisualElement->GetTransform()->originAnchorPoint = center;
    enemyPokemonVisualElement->GetTransform()->position.y = INTERNAL_SCREEN_HEIGHT/2;
    enemyPokemonVisualElement->GetTransform()->position.x = (INTERNAL_SCREEN_WIDTH/2) + 52;
    enemyPokemonVisualElement->flipX = true;
}


void VisualElementSubSystem::SwapPokemonSprites()
{
    playerPokemonVisualElement->SetSpriteIndex(((playerPokemonVisualElement->GetSpriteIndex() + 1) == playerPokemonVisualElement->GetSpriteSheetLength())? 0 : playerPokemonVisualElement->GetSpriteIndex() + 1);
}

VisualElement* VisualElementSubSystem::CreateVisualElement(const char* path, RectTransform rectTransform, int spriteIndex, int columns, int rows)
{
    VisualElement* visualElement = factory->CreateVisualElement(path,rectTransform,spriteIndex,columns,rows);
    currentNonBackgroundVisualElements.push_back(visualElement);
    return visualElement;
}

void VisualElementSubSystem::EarlyUpdate()
{
    SubSystem::EarlyUpdate();
    isDirty = false;
}

void VisualElementSubSystem::LateUpdate()
{
    SubSystem::LateUpdate();
}

bool VisualElementSubSystem::HasBeenDirtied() const
{
    return isDirty;
}


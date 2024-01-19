#pragma once
#include "../SpriteLoader/VisualElementFactory.h"
#include "../SubSystems/SubSystem.h"

class VisualElementSubSystem: public SubSystem
{
public:
    VisualElementSubSystem();
    ~VisualElementSubSystem() override;
    void SetBackground(int spriteIndex);
    void CreateBattleSprites();
    void SwapPokemonSprites();
    VisualElement* backgroundVisualElement{};
    VisualElement* playerPokemonVisualElement{};
    VisualElement* enemyPokemonVisualElement{};
    std::list<VisualElement*> currentNonBackgroundVisualElements{};
    VisualElementFactory* factory{};
    VisualElement* CreateVisualElement(const char* path,RectTransform rectTransform = RectTransform(),int spriteIndex = 0,int columns = 0, int rows = 0);
    VisualElement* BackgoundVisualElement;
    std::set<VisualElement> CurrentNonBackgroundVisualElements;
    void EarlyUpdate() override;
    void LateUpdate() override;
    bool isDirty;
    bool HasBeenDirtied() const;
};

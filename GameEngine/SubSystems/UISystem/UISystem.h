#pragma once
#include <map>
#include <string>
#include "UIGroup.h"
#include "UIGroupType.h"
#include "../SubSystem.h"
#include "../VisualElementSubSystem.h"
#include "../../RenderEngine/RenderEngine.h"


class UISystem : public SubSystem
{
public:
    //Initializes variables
    UISystem();
    //Deallocates memory
    ~UISystem() override;

    void LateUpdate() override;
    //TODO: Most of our path sharing currently use const char pointers. Given what we want to do with data driven stuff, moving to a more flexible string class is probably better
    void SwitchMenu(const char* menuName);
    
    void Free() override;
    void SetActiveGroup(UIGroupType type);
    
    //TODO: Most of our path sharing currently use const char pointers. Given what we want to do with data driven stuff, moving to a more flexible string class is probably better
    std::map<const char*, UIGroup*> uiGroups{};
    UIGroup* currentActiveGroup{};

private:
    VisualElementSubSystem* mVisualElementSubSystem;
    
};

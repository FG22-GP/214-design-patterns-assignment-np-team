#include "UISystem.h"

#include "UIGroup.h"
#include "UIInteractable.h"
#include "../InputSystem.h"
#include "../SubsystemCollection.h"
#include "../../Global.h"

UISystem::UISystem()
{
    printf("Creating uisystem");
    mVisualElementSubSystem = SUBSYSTEM_COLLECTION->GetSubSystem<VisualElementSubSystem>();
}

UISystem::~UISystem() = default;

void UISystem::LateUpdate()
{
    const InputData input_data = SUBSYSTEM_COLLECTION->gInputSystem->inputData;

    const SDL_Point mouse_pointer = SDL_Point{input_data.mouseX, input_data.mouseY};
/*
    if (currentActiveGroup != nullptr)
    {
        for (UIInteractable* const interactable : currentActiveGroup->interactableElements)
        {
            interactable->CheckInteracted(mouse_pointer, input_data.action);
        }
    }
    */
}

void UISystem::SwitchMenu(const char* menuName)
{
    for (auto group_iterator = uiGroups.begin(); group_iterator != uiGroups.end(); ++group_iterator)
    {
        if (strcmp(group_iterator->first, menuName) == 0)
        {
            currentActiveGroup = group_iterator->second;
        }
    }
}

void UISystem::Free()
{
    currentActiveGroup = nullptr;
    uiGroups.clear();
}

void UISystem::SetActiveGroup(UIGroupType type)
{
    switch (type)
    {
    case UIGroupType::battle:
       
        break;

    case UIGroupType::overworld: break;
    }
}



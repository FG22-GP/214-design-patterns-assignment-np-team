#pragma once
#include <SDL_render.h>

struct VisualElement
{

    //List of Renderables?
    //Maybe a type description?
    
    SDL_Rect Rect;
    bool Selectable;

    // Style could be isolated
    SDL_Texture* Texture;
    SDL_Color Color;
};

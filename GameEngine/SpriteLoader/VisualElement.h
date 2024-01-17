#pragma once

#include <SDL_render.h>

#include "RectTransform.h"
#include "TextureLoader.h"
#include "../RenderEngine/ImageQuad.h"

enum DrawMode
{
    simple,
    sliced
};
struct DrawModeData
{
    int topLeft = 0;
    int topRight = 0;
    int bottomLeft = 0;
    int bottomRight = 0;
};

class VisualElement
{
    
    RectTransform mRectTransform;
    GPU_Rect mRect;
    GPU_Rect mClippedRect;
    
    bool mIsClipped;
    int mSpriteIndex;
    int mColumns;
    int mRows;
    int mSpriteSheetLength = 0;

    Texture* mTexture;
    Vector GetOriginDisplacement();
public:
    //Renderer and texture are switched to avoid ambigous function calls;
    VisualElement(Texture* texture,RectTransform rectTransform);
    VisualElement(Texture* texture,int spriteIndex,int columns, int rows,RectTransform rectTransform);
    Vector GetOriginRenderPoint();
    void SetSpriteIndex(int index);
    //void Render();

    bool IsClipped();
    int GetSpriteIndex();
    int GetColumns();
    int GetRows();
    int GetSpriteSheetLength();

    GPU_Rect* GetRenderRect();
    GPU_Rect* GetSrcRect();

    ImageQuad GetImageQuad();
    Texture* GetTexture();
    DrawMode drawMode = simple;
    DrawModeData drawModeData;

    RectTransform* GetTransform();

    bool flipX = false;
    bool flipY = false;
};

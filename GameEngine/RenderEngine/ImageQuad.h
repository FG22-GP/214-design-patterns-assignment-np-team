#pragma once
#include <SDL_gpu.h>

struct ImageQuad
{
    GPU_Image* image;
    GPU_Rect* srcRect;
    GPU_Rect* dstRect;

    bool flipX;
    bool flipY;
};

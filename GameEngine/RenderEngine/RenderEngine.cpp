#include "RenderEngine.h"

#include <algorithm>
#include <cstdio>
#include <SDL.h>

#include "Shader.h"
#include "../Global.h"
#include "../SubSystems/SubsystemCollection.h"
#include "../SubSystems/VisualElementSubSystem.h"

bool RenderEngine::Init()
{
    // Init main buffer
    screen = GPU_Init(SCREEN_WIDTH, SCREEN_HEIGHT, GPU_DEFAULT_INIT_FLAGS | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    screenRect = new GPU_Rect();
    if (!screen)
    {
        printf("SDL_GPU: Window could not be created!\n");
        return false;
    }    
    WINDOW = SDL_GetWindowFromID(screen->context->windowID);
    //SDL_RenderSetLogicalSize(SDL_GetRenderer(gWindow), INTERNAL_SCREEN_WIDTH, INTERNAL_SCREEN_HEIGHT); TODO: Figure out why this doesn't work.
    SDL_SetWindowMinimumSize(WINDOW, INTERNAL_SCREEN_WIDTH, INTERNAL_SCREEN_HEIGHT);
    
    // Init back buffer
    GPU_Image* back_image = GPU_CreateImage(INTERNAL_SCREEN_WIDTH, INTERNAL_SCREEN_HEIGHT, GPU_FORMAT_RGBA);
    backScreen = GPU_LoadTarget(back_image);
    
    GPU_EnableCamera(screen, true);

    errorShader = new Shader("Background", "Resources/Shaders/Error.vert", "Resources/Shaders/Error.frag");
    if (!errorShader->DidCompile())
    {
        return false;
    }
    
    backgroundShader = new Shader("Background", "Resources/Shaders/Background.vert", "Resources/Shaders/Background.frag");
    spriteShader = new Shader("Sprite", "Resources/Shaders/Sprite.vert", "Resources/Shaders/Sprite.frag");
    postProcessShader = new Shader("PostProcessing", "Resources/Shaders/PostProcessing.vert", "Resources/Shaders/PostProcessing.frag");
    SUBSYSTEM_COLLECTION->GetSubSystem<VisualElementSubSystem>();
    
   /* BackgroundImage = GPU_LoadImage("Resources/PokemonSprites/BackgroundTest.png");
    GPU_SetImageFilter(BackgroundImage, GPU_FILTER_NEAREST);
    if (BackgroundImage)
    {
        GPU_GenerateMipmaps(BackgroundImage);
    }*/
    
    debugImage = GPU_LoadImage("Resources/PokemonSprites/Debug.png");
    if (debugImage)
    {
        GPU_SetWrapMode(debugImage, GPU_WRAP_MIRRORED, GPU_WRAP_MIRRORED);
        GPU_GenerateMipmaps(debugImage);
    }

    IS_WINDOW_DIRTY = true;

    return true;
}

void RenderEngine::PreRenderCheck()
{
    if(SUBSYSTEM_COLLECTION->GetSubSystem<VisualElementSubSystem>()->HasBeenDirtied())
    {
        VisualElementSubSystem* subsystem = SUBSYSTEM_COLLECTION->GetSubSystem<VisualElementSubSystem>();
        //printf(__FUNCTION__);
        
        VisualElement* visual_element =  subsystem->backgroundVisualElement;
        backgroundQuad = visual_element->GetImageQuad();
        mElementsToRender.clear();
        mElementsToRender.push_back(subsystem->playerPokemonVisualElement->GetImageQuad());
        mElementsToRender.push_back(subsystem->enemyPokemonVisualElement->GetImageQuad());

        for(auto element : subsystem->currentNonBackgroundVisualElements)
        {
            mElementsToRender.push_back(element->GetImageQuad());
        }
    }
}


void RenderEngine::Render()
{
    constexpr float internal_width = INTERNAL_SCREEN_WIDTH;
    constexpr float internal_height = INTERNAL_SCREEN_HEIGHT;
    
    // Reference: https://github.com/grimfang4/sdl-gpu/blob/master/demos/simple-shader/main.c
    const float time = static_cast<float>(SDL_GetTicks()) * 0.001f;

    // Gets dirtied by input system for now
    if(IS_WINDOW_DIRTY)
    {
        SDL_GetWindowSize(WINDOW, &width, &height);
        GPU_SetWindowResolution(width, height);

        const float ratio_x = width / internal_width;
        const float ratio_y = height / internal_height;
        const float ratio = ratio_x < ratio_y ? ratio_x : ratio_y;

        const float new_width = ratio * internal_width;
        const float new_height = ratio * internal_height;

        const float pad_x = (width - new_width) * 0.5f;
        const float pad_y = (height - new_height) * 0.5f;
        
        screenRect->x = pad_x;
        screenRect->y = pad_y;
        screenRect->w = new_width;
        screenRect->h = new_height;
    }

    GPU_Clear(backScreen);
    GPU_Clear(screen);

    // Background
    if (backgroundShader)
    {
        const auto shader = backgroundShader->DidCompile() ? backgroundShader : errorShader;
        auto back_block = shader->GetBlock();
        GPU_ActivateShaderProgram(shader->GetProgram(), &back_block);
        //GPU_ActivateShaderProgram(0, nullptr);
        shader->SetFloat("Time", time);
        shader->SetVec2("Resolution", internal_width, internal_height);
        shader->SetVec2("TexResolution", backgroundQuad.image->w, backgroundQuad.image->h);
        GPU_BlitRect(backgroundQuad.image,backgroundQuad.srcRect,backScreen,backgroundQuad.dstRect);
        //BlitScreen(BackgroundImage, BackScreen);
    }

    // Sprites
    if (spriteShader)
    {
        const auto shader = spriteShader->DidCompile() ? spriteShader : errorShader;
        auto sprite_block = shader->GetBlock();
        GPU_ActivateShaderProgram(shader->GetProgram(), &sprite_block);
        shader->SetFloat("Time", time);
        shader->SetVec2("Resolution", internal_width, internal_height);
        RenderSprites();
       // GPU_BlitRect(DebugImage, nullptr, BackScreen, spriteRect);
    }

    // UI
    if (userInterfaceShader)
    {
        const auto shader = userInterfaceShader->DidCompile() ? userInterfaceShader : errorShader;
        auto ui_block = shader->GetBlock();
        GPU_ActivateShaderProgram(shader->GetProgram(), &ui_block);
        //BlitScreen(, BackScreen);
    }

    // Post processing: https://github.com/grimfang4/sdl-gpu/issues/240
    constexpr bool post_processing_enabled = true;
    if (postProcessShader)
    {
        const auto shader = postProcessShader->DidCompile() ? postProcessShader : errorShader;
        auto post_block = postProcessShader->GetBlock();
        if (!post_processing_enabled)
        {
            GPU_ActivateShaderProgram(0, nullptr);
        }
        else
        {
            GPU_ActivateShaderProgram(shader->GetProgram(), &post_block);
            shader->SetFloat("Time", time);
            shader->SetVec2("Resolution", width, height);
            shader->SetVec2("TexResolution", internal_width, internal_height);
        }
        GPU_BlitRect(backScreen->image, nullptr, screen, screenRect);
    }
    
    GPU_Flip(screen);
}

void RenderEngine::Quit()
{
    GPU_FreeTarget(screen);
    GPU_FreeTarget(backScreen);
    GPU_FreeShader(backgroundShader->GetProgram());
    GPU_FreeShader(spriteShader->GetProgram());
    GPU_FreeShader(userInterfaceShader->GetProgram());
    GPU_FreeShader(postProcessShader->GetProgram());
    GPU_FreeImage(debugImage);
}

void RenderEngine::BlitScreen(GPU_Image* image, GPU_Target* target)
{
    GPU_BlitRect(image, nullptr, target, nullptr);
}

void RenderEngine::RenderSprites()
{
    for (auto quad : mElementsToRender)
    {
        GPU_FlipEnum flip = (quad.flipX && quad.flipY)? GPU_FLIP_HORIZONTAL | GPU_FLIP_VERTICAL: (quad.flipX)? GPU_FLIP_HORIZONTAL: (quad.flipY)? GPU_FLIP_VERTICAL: GPU_FLIP_NONE;
         GPU_BlitRectX(quad.image,quad.srcRect,backScreen,quad.dstRect,0,0,0,flip);
        //GPU_BlitRect(quad.image,quad.srcRect,backScreen,quad.dstRect);
    }
}



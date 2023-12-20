#include "InputSystem.h"

#include <cstddef>
#include <SDL_events.h>

#include "../Global.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::EarlyUpdate()
{
    SubSystem::EarlyUpdate();

    SDL_Event e;

    inputData.action = false;
    inputData.back = false;
    
    // loop through all pending events from Windows (OS)

    //TODO: Divide this poll queue into two, one for OS non state machine dependent input, such as quit and resize, and one for in game state dependent input, like clicking on a menu or movement
    while (SDL_PollEvent(&e))
    {
        const int sdl_keycode = e.key.keysym.sym;
        switch (e.type)
        {
        case SDL_QUIT: {
                SHOULD_QUIT = true;
            } break;
        case SDL_WINDOWEVENT: {
               if(e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                   IS_WINDOW_DIRTY = true;
            } break;
        
        case SDL_KEYDOWN: {
                if (sdl_keycode == SDLK_LEFT) {
                    inputData.moveX = -1;
                }
                else if(sdl_keycode == SDLK_RIGHT) {
                    inputData.moveX = 1;
                }

                if (sdl_keycode == SDLK_UP) {
                    inputData.moveY = 1;
                }
                else if(sdl_keycode == SDLK_DOWN) {
                    inputData.moveY = -1;
                }
				
            } break;
        case SDL_KEYUP: {
                if ((sdl_keycode == SDLK_LEFT && inputData.moveX < 0) || (sdl_keycode == SDLK_RIGHT && inputData.moveX > 0)) {
                    inputData.moveX = 0;
                }
                if ((sdl_keycode == SDLK_DOWN && inputData.moveY < 0) || (sdl_keycode == SDLK_UP && inputData.moveY > 0))
                {
                    inputData.moveY = 0;
                }
							
            } break;
        case SDL_MOUSEMOTION: {
                inputData.mouseX = e.motion.x;
                inputData.mouseY = e.motion.y;
            } break;
        case SDL_MOUSEBUTTONDOWN: {
                
                inputData.action = e.button.button & SDL_BUTTON_LMASK;
            } break;
        }
    }
}

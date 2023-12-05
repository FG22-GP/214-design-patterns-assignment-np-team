//Using SDL and standard IO
#include <list>
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <xstring>

#include "../ext/SDL2-2.26.4/lib/x64/ExampleClass.h"


//Screen dimension constants. These should be wrapped inside a good way to globally access them. Game Instance should probably recieve them or something
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const char* pikachuImagePath{ "img/pikachu.png" };


//These class references should probably be moved to the UI Manager
SDL_Window* window{};
SDL_Renderer* renderer;

//These variables store details about where the text object will be rendered.
//They should be moved to a struct that can be represented in an visual element object rather than stored as floating values in this class
//Currently they are being set in the LoadText method
int width;
int height;
int textWidth, textHeight;

//These parameters describe the SDL_Rect properties for the pikachu image.
//They should be moved into a struct as part of a visual element bundle
//Currently they are being set in the load image method
int pik_x = 0, pik_y = 0;
int pik_w = 200, pik_h = 200;

SDL_Surface* textSurface;


bool pikachuMoveRight = false;
SDL_Color textColor = { 0xff, 0xff, 0xff };
SDL_Event e;

bool quit = false;

bool Init();
void ProcessEvent(SDL_Event e);
void ProcessInput();
void ProcessGameLogic();
SDL_Texture* LoadText(const char* textToLoad);
SDL_Texture* LoadSprite(const char* imagePath);
void RenderSprite(SDL_Texture* sprite);
void RenderText(SDL_Texture* textToRender);
void ClearScreen();

int main(int argc, char* args[])
{
	//Flag setting
	Init();

	//Example class use and creation
	ExampleClass* example = new ExampleClass();
	example->ExamplePublicVoidMethod();

	//Resource loading
	const char* textToRender = "text_to_render";
	SDL_Texture* TextTexture = LoadText(textToRender);
	SDL_Texture* Example_Sprite = LoadSprite(pikachuImagePath);


	//Update Loop
	while (quit == false)
	{
		SDL_GetTicks(); // can be used, to see, how much time in ms has passed since app start
		// loop through all pending events from Windows (OS)
		while (SDL_PollEvent(&e))
		{
			ProcessEvent(e);
		}
		ProcessInput();
		ProcessGameLogic();
		//DOT, tween animations, or whatever
		ClearScreen();
		//These should belong to 'master' methods, that render all stored renderables on screen. These methods
		//should probably be located in object classes who are responsible for them
		RenderSprite(Example_Sprite);
		RenderText(TextTexture);
		// present screen (switch buffers)
		SDL_RenderPresent(renderer);

		SDL_Delay(0); // can be used to wait for a certain amount of ms
	}

	return 0;
}

SDL_Texture* LoadSprite(const char* imagePath)
{
	SDL_Surface* loadedSurface = IMG_Load(imagePath);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", pikachuImagePath, IMG_GetError());
		return nullptr;
	}
	else
	{
		//Convert surface to screen format
		SDL_Texture* sprite_to_load = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (sprite_to_load == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", pikachuImagePath, SDL_GetError());
			return nullptr;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
		return sprite_to_load;
	}
}

SDL_Texture* LoadText(const char* text_to_render)
{
	// load font
	auto font = TTF_OpenFont("font/lazy.ttf", 100);
	if (font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		return nullptr;
	}
	// render the text into an unoptimized CPU surface
	textSurface = TTF_RenderText_Solid(font, text_to_render, textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return nullptr;
	}
	else
	{
		// Create texture GPU-stored texture from surface pixels
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (textTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			return nullptr
			;
		}
		// Get image dimensions
		width = textSurface->w;
		height = textSurface->h;
		textWidth = textSurface->w;
		textHeight = textSurface->h;
		//Get rid of old loaded surface
		SDL_FreeSurface(textSurface);
		
		return textTexture;
	}
	
}

void RenderSprite(SDL_Texture* spriteToRender)
{
	SDL_Rect targetRectangle{
		pik_x,
		pik_y,
		pik_w,
		pik_h
	};
	SDL_RenderCopy(renderer, spriteToRender, NULL, &targetRectangle);
}

void RenderText(SDL_Texture* textTexture)
{
	SDL_Rect targetRectangle = SDL_Rect{
		500,
		500,
		textWidth,
		textHeight
	};
	SDL_RenderCopy(renderer, textTexture, NULL, &targetRectangle);
}

void RenderText(SDL_Rect* targetRectangle, SDL_Texture* textTexture)
{
	SDL_RenderCopy(renderer, textTexture, NULL, targetRectangle);
}
void ProcessEvent(SDL_Event e)
{
	// check, if it's an event we want to react to:
	switch (e.type) {
	case SDL_QUIT: {
			quit = true;
	} break;

		// This is an example on how to use input events:
		case SDL_KEYDOWN: {
				// input example: if left, then make pikachu move left
				if (e.key.keysym.sym == SDLK_LEFT) {
					pikachuMoveRight = false;
				}
				// if right, then make pikachu move right
				if (e.key.keysym.sym == SDLK_RIGHT) {
					pikachuMoveRight = true;
				}
		} break;
	}

}

void ProcessInput()
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_UP])
	{
		pik_y--;
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		pik_y++;
	}
}

void ProcessGameLogic()
{
	if (pikachuMoveRight) {
		pik_x++;
		if (pik_x > 599) pikachuMoveRight = false;
	}
	else {
		pik_x--;
		if (pik_x < 1) pikachuMoveRight = true;
	}
}

void ClearScreen()
{
	SDL_SetRenderDrawColor(renderer, 120, 60, 255, 255);
	SDL_RenderClear(renderer);
}





bool Init()
{
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	// initialize SDL_ttf for font loading
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}

	//Start up SDL and create window
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	// Create Window and Renderer
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &window, &renderer);
	if (!window)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 1024, 768);



	quit = false;
	printf("Initialising called\n");
	return true;
}



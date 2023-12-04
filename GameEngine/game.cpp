//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//Texture wrapper class
class SpriteSheetTexture
{
public:
	//Initializes variables
	SpriteSheetTexture();

	//Deallocates memory
	~SpriteSheetTexture();

	//Loads image at specified path
	bool loadFromFile(const char* path );

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render( int x, int y, SDL_Rect* clip = NULL );

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene sprites
SDL_Rect gSpriteClips[ 9 ];
SpriteSheetTexture gSpriteSheetTexture;

SpriteSheetTexture::SpriteSheetTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

SpriteSheetTexture::~SpriteSheetTexture()
{
	//Deallocate
	free();
}

bool SpriteSheetTexture::loadFromFile(const char* path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void SpriteSheetTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void SpriteSheetTexture::render( int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int SpriteSheetTexture::getWidth()
{
	return mWidth;
}

int SpriteSheetTexture::getHeight()
{
	return mHeight;
}

const char* pikachuImagePath{ "img/pikachu.png" };
const char* playerSpriteSheetPath{ "img/Player_SpriteSheet.png" };

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
		

		//Create window
		SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &gWindow, &gRenderer);
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			SDL_RenderSetLogicalSize(gRenderer, 256, 256);
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( playerSpriteSheetPath ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
		const int sheet_height = gSpriteSheetTexture.getHeight();
		const int sheet_width = gSpriteSheetTexture.getWidth();

		const int spriteHeight = sheet_height/3;
		const int spriteWidth = sheet_width/3;

		for (int i = 0; i < 9; ++i)
		{
			const int x = i % 3;
			const int y = i / 3;
			
			gSpriteClips[ i ].x = x * spriteWidth;
			gSpriteClips[ i ].y = y * spriteHeight;
			gSpriteClips[ i ].w = spriteWidth;
			gSpriteClips[ i ].h = spriteHeight;
		}	
	}

	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[])
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}

	//Load media
	if( !loadMedia() )
	{
		printf( "Failed to load media!\n" );
	}
	
	// All data related to pikachu
	SDL_Texture* player = NULL; // The final optimized image
	int yInput = 0;
	int xInput = 0;
	int spriteIndex = 0;
	int player_x, player_y;
	player_x = player_y = 0;

	// load font
	auto font = TTF_OpenFont("font/lazy.ttf", 100);
	if (font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		return -1;
	}

	// create text from font
	SDL_Color textColor = { 0xff, 0xff, 0xff };
	//Render text surface
	SDL_Texture* textTexture; // The final optimized image

	// render the text into an unoptimized CPU surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, "The lazy fox, blah blah", textColor);
	int textWidth, textHeight;
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return -1;
	}
	else
	{
		// Create texture GPU-stored texture from surface pixels
		textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (textTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			return -1;
		}
		// Get image dimensions
		auto width = textSurface->w;
		auto height = textSurface->h;
		textWidth = textSurface->w;
		textHeight = textSurface->h;
		//Get rid of old loaded surface
		SDL_FreeSurface(textSurface);
	}

	SDL_Event e; bool quit = false;

	// while the user doesn't want to quit
	while (quit == false)
	{
		SDL_GetTicks(); // can be used, to see, how much time in ms has passed since app start

		// loop through all pending events from Windows (OS)
		while (SDL_PollEvent(&e))
		{
			// check, if it's an event we want to react to:
			switch (e.type) {
				case SDL_QUIT: {
					quit = true;
				} break;

				case SDL_KEYDOWN: {
					if (e.key.keysym.sym == SDLK_LEFT) {
						xInput = -1;
					}
					else if(e.key.keysym.sym == SDLK_RIGHT) {
						xInput = 1;
					}

					if (e.key.keysym.sym == SDLK_UP) {
						yInput = 1;
					}
					else if(e.key.keysym.sym == SDLK_DOWN) {
						yInput = -1;
					}
				
				} break;

				case SDL_KEYUP: {
					if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) {
						xInput = 0;
					}

					if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN) {
						yInput = 0;
					}
							
				} break;
			} 
		}

		if(xInput != 0 || yInput != 0)
		{
			player_x += xInput;
			player_y -= yInput;

			if(yInput != 0)
			{
				spriteIndex = yInput > 0 ? 6 : 0;
			}
			else if(xInput != 0)
			{
				spriteIndex = 3; 
			}
		}
		
		// clear the screen
		SDL_SetRenderDrawColor(gRenderer, 120, 60, 255, 255);
		SDL_RenderClear(gRenderer);
		
		gSpriteSheetTexture.render( player_x, player_y, &gSpriteClips[ spriteIndex ] );

		/*

		// render the text
		targetRectangle = SDL_Rect{
			500,
			500,
			textWidth,
			textHeight
		};
		SDL_RenderCopy(gRenderer, textTexture, NULL, &targetRectangle); */

		// present screen (switch buffers)
		SDL_RenderPresent(gRenderer);

		SDL_Delay(10.f); // can be used to wait for a certain amount of ms
	}

	close();

	return 0;
}
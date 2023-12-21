#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 500;

int setupSDL();

int main(int, char**) {
	printf("hello, world");

	if (setupSDL() != 0) {
		printf("Unable to setup correctly");
	}

	bool quit = false;
	SDL_Event event;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}


  SDL_Rect dest = {0,0,500,500};

	SDL_RenderCopy(renderer, texture, NULL, &dest);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	return 0;
}

int setupSDL() {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("QuadTree Collision Detection demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_Log("Error creating SDL_Renderer!");
		return 1;
	}



	// SDL_Image
	int imgFlags = IMG_INIT_PNG || IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image Error: %s\n", IMG_GetError());
		return 2;
	}

  // load test texture
	texture = IMG_LoadTexture(renderer, "assets/test.png");
	if (texture == NULL) {
		printf("Couldn't load player texture. %s", IMG_GetError());
    return 3;
	}

  return 0;
}

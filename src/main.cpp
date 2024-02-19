#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
TTF_Font* font = NULL;

SDL_Joystick* controller;

int WINDOW_WIDTH = 1080;
int WINDOW_HEIGHT = 720;

int setupSDL();

int main(int, char**) {
    printf("hello, world");

    if (setupSDL() != 0) {
        printf("Unable to setup correctly");
        return -1;
    }

    // Ensure there's a controller connected, and set the variable.
    if(SDL_NumJoysticks() < 1){
        printf("no joysticks found");
        return -12;
    }
    controller = SDL_JoystickOpen(0); // TODO: nullcheck this

    bool quit = false;
    SDL_Event event;
    

    // vsync stuff
    Uint32 startOfFrame = 0;
    const float TICKS_PER_FRAME = (1000.f / 60.f);


    while (!quit) {
        startOfFrame = SDL_GetTicks();

        // Input
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT: 
                {
                    quit = true;
                    break;
                }
                case SDL_JOYAXISMOTION:
                {
                    printf("xbox axis movement at %i\n", startOfFrame);
                    printf("event.jaxis.value: %i\n", event.jaxis.value);
                    printf("event.jaxis.axis: %i\n", event.jaxis.axis);
                    break;
                }
                case SDL_JOYBUTTONUP:
                {
                    printf("xbox btnup at %i\n", startOfFrame);
                    break;
                }
                case SDL_JOYBUTTONDOWN:
                {
                    printf("xbox btndown at %i\n", startOfFrame);
                    break;
                }
            }
        }


        // Render
        // TODO: show values of gamepad sticks as they move live.
        SDL_Rect dest = { 0,0,500,500 };

        SDL_RenderCopy(renderer, texture, NULL, &dest);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        // Wait for remaining ticks in frame.
        Uint32 frameLength = SDL_GetTicks() - startOfFrame;
        if (frameLength < TICKS_PER_FRAME){
            float remainingTimeOfFrame = TICKS_PER_FRAME - frameLength;
            SDL_Delay(remainingTimeOfFrame);
        }
    }
    return 0;
}

int setupSDL() {

    // SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize our window with the rendering context in the window.
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

    // SDL ttf (initialize font variable)
	if (TTF_Init() == -1) {
		printf("ttf couldn't setup\n");
		return 3;
	}
	font = TTF_OpenFont("assets/Aver.ttf", 26);
	if (font == NULL) {
		printf("problem loading font\n");
		return 4;
	}


    // load test texture
    texture = IMG_LoadTexture(renderer, "assets/test.png");
    if (texture == NULL) {
        printf("Couldn't load player texture. %s", IMG_GetError());
        return 3;
    }

    return 0;
}

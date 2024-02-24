#include <stdio.h>
#include <string>

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
SDL_Joystick* getFrame1AsJoystick();

int main(int, char**) {

    if (setupSDL() != 0) {
        printf("Unable to setup correctly");
        return -1;
    }
    controller = getFrame1AsJoystick();
    if(controller == nullptr){
        printf("Error trying to get frame1.\n");
        return -2;
    }

    printf("----------\n");
    printf("SELECTED JOYSTICK '%s', device details:\n", SDL_JoystickName(controller));
    printf("joystick name: %s\n", SDL_JoystickName(controller));
    printf("usb vendor id: %i\n", SDL_JoystickGetVendor(controller));
    Uint16 id = SDL_JoystickGetProduct(controller);
    printf("product id: %i\n", id);
    printf("product version:%i\n", SDL_JoystickGetProductVersion(controller));
    printf("firmware version:%i\n", SDL_JoystickGetFirmwareVersion(controller));
    printf("----------\n");

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

SDL_Joystick* getFrame1AsJoystick(){
    SDL_Joystick* controller = nullptr;

    // Ensure there's a controller connected, and set the variable.
    if(SDL_NumJoysticks() < 1){
        printf("No joysticks found\n");
        return nullptr;
    }

    printf("Listing joysticks:\n");
    int frame1Index = -1;
    for(int i=0; i<SDL_NumJoysticks(); i++){
        // Check joystick name and product id.
        // For now, just going to use joystick id
        controller = SDL_JoystickOpen(i);
        std::string name = SDL_JoystickName(controller);
        Uint16 id = SDL_JoystickGetProduct(controller);
        printf("(name: '%s', id: %i)\n", name.c_str(), id);
        if(name == "Frame1"){// && id == 22352){
            frame1Index = i;
        }
    }

    if(frame1Index < 0){
        return nullptr;
    }

    controller = SDL_JoystickOpen(frame1Index);
    return controller;
}

int setupSDL() {
    // So this window doesn't have to be focused.
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

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

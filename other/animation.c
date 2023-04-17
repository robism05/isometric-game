#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_FRAMES 10

typedef struct {
    SDL_Texture* frames[MAX_FRAMES];   // Array of textures for each frame
    int numFrames;                     // Number of frames in the animation
    int currentFrame;                 // Current frame of the animation
    int frameWidth;                   // Width of each frame
    int frameHeight;                  // Height of each frame
    int frameDuration;               // Duration of each frame in milliseconds
    int elapsedTime;                // Elapsed time since last frame change
} PlayerAnimation;


// function prototypes
PlayerAnimation LoadIdleAnimation(SDL_Renderer* renderer);
PlayerAnimation LoadRunAnimation(SDL_Renderer* renderer);
void UpdatePlayerAnimation(PlayerAnimation* animation, int deltaTime);
void RenderPlayerAnimation(PlayerAnimation* animation, SDL_Renderer* renderer, int x, int y);


// Example loading function for a player idle animation with multiple frames
PlayerAnimation LoadIdleAnimation(SDL_Renderer* renderer) 
{
    PlayerAnimation idleAnimation;
    idleAnimation.numFrames = 6;
    idleAnimation.currentFrame = 0;
    idleAnimation.frameWidth = 128;
    idleAnimation.frameHeight = 128;
    idleAnimation.frameDuration = 200; // 200ms per frame
    idleAnimation.elapsedTime = 0;

    // Load textures for each frame
    for (int i = 0; i < idleAnimation.numFrames; i++) {
        char filename[60];
        sprintf(filename, "../res/textures/animation/player_idle/player-idle%d.png", i + 1);
        idleAnimation.frames[i] = IMG_LoadTexture(renderer, filename);
    }

    return idleAnimation;
}

// function for loading run animation when player runs
PlayerAnimation LoadRunAnimation(SDL_Renderer* renderer)
{
    PlayerAnimation runAnimation;
    runAnimation.numFrames = 3;
    runAnimation.currentFrame = 0;
    runAnimation.frameWidth = 128;
    runAnimation.frameHeight = 128;
    runAnimation.frameDuration = 200;
    runAnimation.elapsedTime = 0;

    for (int i = 0; i < runAnimation.numFrames; i++){
        char filename[60];
        sprintf(filename, "../res/textures/animation/player_run/player-run%d.png", i + 1);
        runAnimation.frames[i] = IMG_LoadTexture(renderer, filename);
    }
    return runAnimation;
}

void UpdatePlayerAnimation(PlayerAnimation* animation, int deltaTime) 
{
    animation->elapsedTime += deltaTime;
    if (animation->elapsedTime >= animation->frameDuration) {
        animation->elapsedTime = 0;
        animation->currentFrame = (animation->currentFrame + 1) % animation->numFrames;
    }
}

void RenderPlayerAnimation(PlayerAnimation* animation, SDL_Renderer* renderer, int x, int y) 
{
    SDL_Rect srcRect = { 0, 0, animation->frameWidth, animation->frameHeight };
    SDL_Rect destRect = { x, y, animation->frameWidth, animation->frameHeight };
    SDL_RenderCopy(renderer, animation->frames[animation->currentFrame], &srcRect, &destRect);
}


// Don't forget to free the loaded textures when they are no longer needed
void FreePlayerAnimation(PlayerAnimation* animation) 
{
    for (int i = 0; i < animation->numFrames; i++) {
        SDL_DestroyTexture(animation->frames[i]);
    }
}


int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create SDL window and renderer
    SDL_Window* window = SDL_CreateWindow("Player Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    if (window == NULL) {
        printf("Failed to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        printf("Failed to create SDL renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load idle animation
    PlayerAnimation idleAnimation = LoadIdleAnimation(renderer);
    PlayerAnimation runAnimation = LoadRunAnimation(renderer);


    int playerX = 100;
    int playerY = 100;



    // Game loop
    bool quit = false;
    Uint32 prevTime = SDL_GetTicks();
    while (!quit){
        SDL_Event event;

        // Update animation
        Uint32 currentTime = SDL_GetTicks();
        int deltaTime = currentTime - prevTime;
        prevTime = currentTime;
        UpdatePlayerAnimation(&idleAnimation, deltaTime);

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render animation
        RenderPlayerAnimation(&idleAnimation, renderer, playerX, playerY);

        
        // event loop
        while (SDL_PollEvent(&event)){
            SDL_RenderClear(renderer);
            if (event.type == SDL_QUIT){
                quit = true;
            }

            if (event.type == SDL_KEYDOWN){
                int key = event.key.keysym.sym;
                if (event.key.keysym.sym == SDLK_LEFT){
                    playerX -= 20;
                    //RenderPlayerAnimation(&runAnimation, renderer, playerX, playerY);
                } else if (event.key.keysym.sym == SDLK_RIGHT){
                    playerX += 20;
                    //RenderPlayerAnimation(&runAnimation, renderer, playerX, playerX);
                } 
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay to achieve desired frame rate
        SDL_Delay(16);
    }

    // Free resources
    FreePlayerAnimation(&idleAnimation);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

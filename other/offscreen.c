#include <stdio.h>
#include <stdbool.h>

#include "lib/SDL2/SDL.h"
#include "lib/SDL2/SDL_image.h"


#define BACKGROUND_IMAGE "res/textures/grid.png"
#define MAP_W 400
#define MAP_H 300
#define BLOCK_SIZE 32
#define SCALE 3

// scale up our game thingies
#define SCREEN_WIDTH (MAP_W * SCALE)
#define SCREEN_HEIGHT (MAP_H * SCALE)

int player_size = (64 * SCALE);
int moveX = (BLOCK_SIZE * SCALE);
int moveY = (BLOCK_SIZE * SCALE) / 2;

// Function prototypes
void handleInput(SDL_Rect *playerRect);
void updateBackground(SDL_Rect *playerRect, SDL_Rect *backgroundRect);

// main function duh
int main() {

    // initialize SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr,"SDL2 could not be initialized! SDL_Error: %s\n",SDL_GetError());
        exit(1);      
    } //else { printf("SDL initialized\n"); } 
    if (IMG_Init(IMG_INIT_PNG) < 0){
        fprintf(stderr,"SDL_image failed to load: %s\n",IMG_GetError());
    } //else { printf("SDL_image initialized\n"); }

    // create window and give context
    SDL_Window* window = SDL_CreateWindow(
        "Game", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);


    // create game objects
    
    int playerX = BLOCK_SIZE * SCALE;
    int playerY = BLOCK_SIZE * SCALE;
    SDL_Rect playerRect = { playerX, playerY, player_size, player_size };
    SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 };

    // Load images
    SDL_Surface* playerSurface = IMG_Load("res/textures/player.png");
    SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);

    SDL_Surface* backgroundSurface = IMG_Load(BACKGROUND_IMAGE);
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);


    SDL_Event event;

    // Game loop
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Handle input
        handleInput(&playerRect);

        // Update background
        updateBackground(&playerRect, &backgroundRect);

        // Clear renderer
        SDL_RenderClear(renderer);

        // Draw background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Draw player
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

        // Update renderer
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

void handleInput(SDL_Rect *playerRect) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP]) {
        playerRect->y += 1;
        playerRect->x += 1;
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        playerRect->y -= 1;
        playerRect->x -= 1;
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        playerRect->x -= 2;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        playerRect->x += 2;
    }
}

void updateBackground(SDL_Rect *playerRect, SDL_Rect *backgroundRect) {
    if (playerRect->x < 0) {
        backgroundRect->x += 1;
    } else if (playerRect->x + playerRect->w > SCREEN_WIDTH) {
        backgroundRect->x -= 1;
    }
    if (playerRect->y < 0) {
        backgroundRect->y += 1;
    } else if (playerRect->y + playerRect->h > SCREEN_HEIGHT) {
        backgroundRect->y -= 1;
    }
}

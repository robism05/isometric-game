#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdbool.h>

// Window dimensions
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Player dimensions
#define PLAYER_SIZE 32

// Player position and velocity
int player_x = 0;
int player_y = 0;
int player_vel_x = 0;
int player_vel_y = 0;

// SDL variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* playerTexture = NULL;


// Function to load an image as an SDL_Texture
SDL_Texture* loadTexture(const char* imagePath, SDL_Renderer* renderer) {

    // Load image using SDL_image
    SDL_Surface* imageSurface = IMG_Load(imagePath);
    if (!imageSurface) {
        fprintf(stderr,"Failed to load image '%s'. SDL_image Error: %s\n", imagePath, IMG_GetError());
        return NULL;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (!texture) {
        fprintf(stderr,"Failed to create texture from surface. SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(imageSurface);
        return NULL;
    }

    // Free surface
    SDL_FreeSurface(imageSurface);
    return texture;
}


int main(int argc, char* args[]) {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL. SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Failed to initialize SDL_image. SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Create window
    window = SDL_CreateWindow(
    "Player Movement", 
    SDL_WINDOWPOS_UNDEFINED, 
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, 
    SCREEN_HEIGHT, 
    SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr,"Failed to create window. SDL Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr,"Failed to create renderer. SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load player texture
    playerTexture = loadTexture("../res/textures/player.png", renderer);
    if (!playerTexture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Game loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Game loop
    while (!quit) {

        // Update player position
        player_x += player_vel_x;
        player_y += player_vel_y;

        // Clear renderer
        SDL_RenderClear(renderer);

        // Update player texture position
        SDL_Rect dstRect = { player_x, player_y, PLAYER_SIZE, PLAYER_SIZE };

        // Render player texture
        SDL_RenderCopy(renderer, playerTexture, NULL, &dstRect);

        // Update screen

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                // Handle player movement
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        player_vel_y = -1;
                        break;
                    case SDLK_DOWN:
                        player_vel_y = 1;
                        break;
                    case SDLK_LEFT:
                        player_vel_x = -1;
                        // Flip player texture horizontally
                        SDL_RenderCopyEx(renderer, playerTexture, NULL, &dstRect, 0, NULL, SDL_FLIP_HORIZONTAL);
                        break;
                    case SDLK_RIGHT:
                        player_vel_x = 1;
                        // Reset player texture flip
                        SDL_RenderCopy(renderer, playerTexture, NULL, &dstRect);
                        break;
                    default:
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                // Handle player movement stop
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_DOWN:
                        player_vel_y = 0;
                        break;
                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        player_vel_x = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_RenderPresent(renderer);

    }

    // Clean up
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

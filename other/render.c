// trying to resize textures with the window

#include "../src/util/util.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char* argv[]) 
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect texture_rect;
    
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL2 could not be initialized! SDL_Error: %s\n", SDL_GetError());
        return 1;
    } else { printf("SDL_Init: %d\n",SDL_Init(SDL_INIT_VIDEO)); }

    // Create window
    window = SDL_CreateWindow(
        "Grass Texture", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        640, 480, 
        SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    } else { printf("window: %s\n",window); }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load texture
    SDL_Surface* surface = IMG_Load("../res/textures/grass.png");
    if (surface == NULL) {
        printf("Texture could not be loaded! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_FreeSurface(surface);

    // Initialize texture position and size
    texture_rect.x = 0;
    texture_rect.y = 0;
    texture_rect.w = 100;
    texture_rect.h = 100;

    // Run game loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        // Update texture size
                        texture_rect.w = event.window.data1 / 2;
                        texture_rect.h = event.window.data2 / 2;
                    }
                    break;
            }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render texture
        SDL_RenderCopy(renderer, texture, NULL, &texture_rect);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


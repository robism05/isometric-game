#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

void renderTexture(int x, int y) 
{
    // Clear the renderer
    SDL_RenderClear(renderer);

    // Copy the texture to the renderer at the mouse cursor's location
    SDL_Rect dstRect = {x, y, 64, 64}; // Set the destination rectangle to the mouse cursor's location
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    // Update the screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    window = SDL_CreateWindow(
        "SDL2 PNG",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN);

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load the PNG image
    SDL_Surface* surface = IMG_Load("../res/textures/iso-grass-top.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Main loop
    SDL_Event event;
    int mouseX = 0, mouseY = 0; // Variables to store mouse cursor's position

    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
        else if (event.type == SDL_MOUSEMOTION) {
            // Update the mouse cursor's position
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            
            // Render the texture at the mouse cursor's location
            if (mouseX==mouseY){
                printf("mouseX: %d\nmouseY: %d\n",mouseX,mouseY);
                renderTexture(mouseX, mouseY);
            }
        }
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


#include <SDL2/SDL.h>

#define GRID_SIZE 10
#define SQUARE_SIZE 32

#define SCREEN_WIDTH GRID_SIZE * SQUARE_SIZE
#define SCREEN_HEIGHT GRID_SIZE * SQUARE_SIZE

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

int highlightedX = -1; // Grid X coordinate of the highlighted square
int highlightedY = -1; // Grid Y coordinate of the highlighted square

void renderGrid() {
    // Clear the renderer
    SDL_RenderClear(renderer);

    // Render the grid
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = 0; y < GRID_SIZE; ++y) {
            // Set the color based on whether the square is highlighted or not
            SDL_Color color;
            if (x == highlightedX && y == highlightedY) {
                color = (SDL_Color){255, 255, 0, 255}; // Yellow for highlighted square
            } else {
                color = (SDL_Color){0, 0, 0, 255}; // Black for inactive squares
            }

            // Render the square
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_Rect rect = {x * SQUARE_SIZE, y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    window = SDL_CreateWindow(
        "SDL2 Grid",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Main loop
    SDL_Event event;

    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
        else if (event.type == SDL_MOUSEMOTION) {
            // Update the state of the highlighted square
            int mouseX = event.motion.x / SQUARE_SIZE; // Get the grid X coordinate from mouse X position
            int mouseY = event.motion.y / SQUARE_SIZE; // Get the grid Y coordinate from mouse Y position

            if (highlightedX != mouseX || highlightedY != mouseY) {
                // Only update if the highlighted square has changed
                highlightedX = mouseX;
                highlightedY = mouseY;
                renderGrid(); // Render the updated grid
            }
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


// includes
#include <stdio.h>
#include <SDL2/SDL.h>

// macro defines
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BLOCK_SIZE 20


// main code
void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) 
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void draw_square(SDL_Renderer *renderer, int x, int y) 
{
    draw_line(renderer, x, y, x + BLOCK_SIZE, y);
    draw_line(renderer, x + BLOCK_SIZE, y, x + BLOCK_SIZE, y + BLOCK_SIZE);
    draw_line(renderer, x + BLOCK_SIZE, y + BLOCK_SIZE, x, y + BLOCK_SIZE);
    draw_line(renderer, x, y + BLOCK_SIZE, x, y);
}

int main(int argc, char *argv[]) 
{
    // initialise SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Window *window = SDL_CreateWindow(
        "Isometric View of Square",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderClear(renderer);

    // Draw isometric grid
    int grid_width = SCREEN_WIDTH / BLOCK_SIZE;
    int grid_height = SCREEN_HEIGHT / BLOCK_SIZE;
    int offset_x = (SCREEN_WIDTH - grid_width * BLOCK_SIZE) / 2;
    int offset_y = (SCREEN_HEIGHT - grid_height * BLOCK_SIZE) / 2;
    for (int i = 0; i < grid_width; i++) {
        for (int j = 0; j < grid_height; j++) {
            int x = offset_x + i * BLOCK_SIZE - j * BLOCK_SIZE;
            int y = offset_y + i * BLOCK_SIZE + j * BLOCK_SIZE;
            printf("\nx[%d]: %d\ny[%d]: %d",x,i, y,i);
            draw_square(renderer, x, y);
        }
    }

    /*
    draw_line(renderer, 200, 100, 300, 150); // top line
    draw_line(renderer, 300, 150, 200, 200); // right line
    draw_line(renderer, 200, 200, 100, 150); // bottom line
    draw_line(renderer, 100, 150, 200, 100); // left line
    */

    SDL_RenderPresent(renderer);


    SDL_Event event;
    while(1) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT:
                return 0;
            }
        }        
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
